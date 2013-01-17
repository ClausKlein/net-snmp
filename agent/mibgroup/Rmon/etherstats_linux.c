/*
 * standard Net-SNMP includes
 */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/*
 * include our parent header
 */
#include "agutil_api.h"
#include "etherstats_linux.h"

/*
 * @retval  0 success
 * @retval -1 getifaddrs failed
 * @retval -2 memory allocation failed
 */

struct ifname *
etherstats_interface_name_list_get(struct ifname *list_head, int *retval) {
    struct ifaddrs *addrs = NULL, *p = NULL;
    struct ifname *nameptr1 = NULL, *nameptr2 = NULL;

    DEBUGMSGTL(("access:etherStatsTable:interface_name_list_get",
                "called\n"));

    if ((getifaddrs(&addrs)) < 0) {
        DEBUGMSGTL(("access:etherStatsTable:interface_name_list_get",
                    "getifaddrs failed\n"));
        snmp_log(LOG_ERR, "access:etherStatsTable,interface_name_list_get, getifaddrs failed\n");
        *retval = -1;
        return NULL;
    }

    for (p = addrs; p; p = p->ifa_next) {
        if (!list_head) {
            if ((list_head = (struct ifname *) malloc(sizeof(struct ifname))) < 0) {
                DEBUGMSGTL(("access:etherStatsTable:interface_name_list_get",
                            "memory allocation failed\n"));
                snmp_log(LOG_ERR, "access:etherStatsTable,interface_name_list_get, memory allocation failed\n");
                freeifaddrs(addrs);
                *retval = -2;
                return NULL;
            }
            memset(list_head, 0, sizeof(struct ifname));
            strlcpy(list_head->name, p->ifa_name, IF_NAMESIZE);
            continue;
        }
        for (nameptr1 = list_head; nameptr1; nameptr2 = nameptr1, nameptr1 = nameptr1->ifn_next)
            if (!strncmp(p->ifa_name, nameptr1->name, IF_NAMESIZE)) {
                break;
            }

        if (nameptr1) {
            continue;
        }

        if ((nameptr2->ifn_next = (struct ifname *) malloc(sizeof(struct ifname))) < 0) {
            DEBUGMSGTL(("access:etherStatsTable:interface_name_list_get",
                        "memory allocation failed\n"));
            snmp_log(LOG_ERR, "access:etherStatsTable,interface_name_list_get, memory allocation failed\n");
            etherstats_interface_name_list_free(list_head);
            freeifaddrs(addrs);
            *retval = -2;
            return NULL;
        }
        nameptr2 = nameptr2->ifn_next;
        memset(nameptr2, 0, sizeof(struct ifname));
        strlcpy(nameptr2->name, p->ifa_name, IF_NAMESIZE);
        continue;
    }

    freeifaddrs(addrs);
    *retval = 0;
    return list_head;
}


/*
 * @retval 0 success
 * @retval -1 invalid pointer
 */

int
etherstats_interface_name_list_free(struct ifname *list_head) {
    struct ifname *nameptr1 = NULL, *nameptr2 = NULL;

    DEBUGMSGTL(("access:etherStatsTable:interface_name_list_free",
                "called\n"));

    if (!list_head) {
        snmp_log(LOG_ERR, "access:etherStatsTable:interface_name_list_free: invalid pointer list_head");
        DEBUGMSGTL(("access:etherStatsTable:interface_name_list_free",
                    "invalid pointer list_head\n"));
        return -1;
    }

    for (nameptr1 = list_head; nameptr1; nameptr1 = nameptr2) {
        nameptr2 = nameptr1->ifn_next;
        free(nameptr1);
    }

    return 0;
}

/*
 * @retval  0 : not found
 * @retval !0 : ifIndex
 */

int
etherstats_interface_ioctl_ifindex_get(int fd, const char *name) {
#ifndef SIOCGIFINDEX
#warning "SIOCGIFINDEX not defined"
    return 0;
#else
    struct ifreq    ifrq;
    int rc = 0;

    DEBUGMSGTL(("access:etherStatsTable:ioctl", "ifindex_get\n"));

    rc = _etherStats_ioctl_get(fd, SIOCGIFINDEX, &ifrq, name);
    if (rc < 0) {
        DEBUGMSGTL(("access:etherStats:ioctl",
                    "ifindex_get error on inerface '%s'\n", name));
        snmp_log(LOG_ERR, "access:etherStatsTable:ioctl, ifindex_get error on inerface '%s'\n", name);
        return 0;

    }

    return ifrq.ifr_ifindex;
#endif /* SIOCGIFINDEX */
}

/*
 * @retval  0 success
 * @retval -1 cannot get ETHTOOL_DRVINFO failed
 * @retval -2 n_stats zero - no statistcs available
 * @retval -3 memory allocation for holding the statistics failed
 * @retval -4 cannot get ETHTOOL_GSTRINGS information
 * @retval -5 cannot get ETHTOOL_GSTATS information
 * @retval -6 function not supported if HAVE_LINUX_ETHTOOL_H not defined
 */

int
interface_ioctl_etherstats_get(ETH_STATS_T *data , int fd, const char *name) {

#ifdef HAVE_LINUX_ETHTOOL_H

    struct ethtool_drvinfo driver_info;
    struct ethtool_gstrings *eth_strings;
    struct ethtool_stats *eth_stats;
    struct ifreq ifr;
    unsigned int nstats, size_str, i;
    int err;

    DEBUGMSGTL(("access:etherStatsTable:interface_ioctl_etherstats_get",
                "called\n"));

    memset(&ifr, 0, sizeof(ifr));
    strlcpy(ifr.ifr_name, name, sizeof(ifr.ifr_name));

    memset(&driver_info, 0, sizeof(driver_info));
    driver_info.cmd = ETHTOOL_GDRVINFO;
    ifr.ifr_data = (char *)&driver_info;

    err = _etherStats_ioctl_get(fd, SIOCETHTOOL, &ifr, name);
    if (err < 0) {
        DEBUGMSGTL(("access:etherStatsTable:interface_ioctl_etherstats_get",
                    "ETHTOOL_GETDRVINFO failed on interface |%s| \n", name));
        return -1;
    }

    nstats = driver_info.n_stats;
    if (nstats < 1) {
        DEBUGMSGTL(("access:etherStatsTable:interface_ioctl_etherstats_get",
                    "no stats available for interface |%s| \n", name));
        return -2;
    }

    size_str = nstats * ETH_GSTRING_LEN;

    eth_strings = malloc(size_str + sizeof(struct ethtool_gstrings));
    if (!eth_strings) {
        DEBUGMSGTL(("access:etherStatsTable:interface_ioctl_etherstats_get",
                    "no memory available\n"));
        snmp_log(LOG_ERR, "access:etherStatsTable,interface_ioctl_etherstats_get, no memory available\n");

        return -3;
    }
    memset(eth_strings, 0, (size_str + sizeof(struct ethtool_gstrings)));

    eth_stats = malloc(size_str + sizeof(struct ethtool_stats));
    if (!eth_stats) {
        free(eth_strings);
        DEBUGMSGTL(("access:etherStatsTable:interface_ioctl_etherstats_get",
                    "no memory available\n"));
        snmp_log(LOG_ERR, "access:etherStatsTable,interface_ioctl_etherstats_get, no memory available\n");

        return -3;
    }
    memset(eth_stats, 0, (size_str + sizeof(struct ethtool_stats)));

    eth_strings->cmd = ETHTOOL_GSTRINGS;
    eth_strings->string_set = ETH_SS_STATS;
    eth_strings->len = nstats;
    ifr.ifr_data = (char *) eth_strings;

    err = _etherStats_ioctl_get(fd, SIOCETHTOOL, &ifr, name);
    if (err < 0) {
        DEBUGMSGTL(("access:etherStatsTable:interface_ioctl_etherstats_get",
                    "cannot get stats strings information for interface |%s| \n", name));
        snmp_log(LOG_ERR, "access:etherStatsTable,interface_ioctl_etherstats_get, cannot get stats strings information for interface |%s| \n", name);

        free(eth_strings);
        free(eth_stats);
        return -4;
    }

    eth_stats->cmd = ETHTOOL_GSTATS;
    eth_stats->n_stats = nstats;
    ifr.ifr_data = (char *) eth_stats;
    err = _etherStats_ioctl_get(fd, SIOCETHTOOL, &ifr, name);
    if (err < 0) {
        DEBUGMSGTL(("access:etherStatsTable:interface_ioctl_etherstats_get",
                    "cannot get stats strings information for interface |%s| \n", name));
        snmp_log(LOG_ERR, "access:etherStatsTable,interface_ioctl_etherstats_get, cannot get stats information for interface |%s| \n", name);

        free(eth_strings);
        free(eth_stats);
        return -5;
    }

    for (i = 0; i < nstats; i++) {
        char s[ETH_GSTRING_LEN];

        strlcpy(s, (const char *) &eth_strings->data[i * ETH_GSTRING_LEN], sizeof(s));

        if (ETHERSTATSOCTETS(s)) {
            data->octets += (u_long)eth_stats->data[i];
        }
        if (ETHERSTATSPKTS(s)) {
            data->packets += (u_long)eth_stats->data[i];
        }
        if (ETHERSTATSBROADCASTPKTS(s)) {
            data->bcast_pkts += (u_long)eth_stats->data[i];
        }
        if (ETHERSTATSMULTICASTPKTS(s)) {
            data->mcast_pkts = (u_long)eth_stats->data[i];
        }
        if (ETHERSTATSCRCALIGNERRORS(s)) {
            data->crc_align += (u_long)eth_stats->data[i];
        }
        if (ETHERSTATSUNDERSIZEPKTS(s)) {
            data->undersize += (u_long)eth_stats->data[i];
        }
        if (ETHERSTATSOVERSIZEPKTS(s)) {
            data->oversize += (u_long)eth_stats->data[i];
        }
        if (ETHERSTATSFRAGMENTS(s)) {
            data->fragments += (u_long)eth_stats->data[i];
        }
        if (ETHERSTATSJABBERS(s)) {
            data->jabbers += (u_long)eth_stats->data[i];
        }
        if (ETHERSTATSCOLLISIONS(s)) {
            data->collisions += (u_long)eth_stats->data[i];
        }
        if (ETHERSTATSPKTS64OCTETS(s)) {
            data->pkts_64 += (u_long)eth_stats->data[i];
        }
        if (ETHERSTATSPKTS65TO127OCTETS(s)) {
            data->pkts_65_127 += (u_long)eth_stats->data[i];
        }
        if (ETHERSTATSPKTS128TO255OCTETS(s)) {
            data->pkts_128_255 += (u_long)eth_stats->data[i];
        }
        if (ETHERSTATSPKTS256TO511OCTETS(s)) {
            data->pkts_256_511 += (u_long)eth_stats->data[i];
        }
        if (ETHERSTATSPKTS512TO1023OCTETS(s)) {
            data->pkts_512_1023 += (u_long)eth_stats->data[i];
        }
        if (ETHERSTATSPKTS1024TO1518OCTETS(s)) {
            data->pkts_1024_1518 += (u_long)eth_stats->data[i];
        }
    }
    free(eth_strings);
    free(eth_stats);

    return 0;
#else
    return -6;
#endif

}


/* ioctl wrapper
 *
 * @param fd : socket fd to use w/ioctl, or -1 to open/close one
 * @param which
 * @param ifrq
 * param ifentry : ifentry to update
 * @param name
 *
 * @retval  0 : success
 * @retval -1 : invalid parameters
 * @retval -2 : couldn't create socket
 * @retval -3 : ioctl call failed
 */
int
_etherStats_ioctl_get(int fd, int which, struct ifreq *ifrq, const char* name) {
    int ourfd = -1, rc = 0;

    DEBUGMSGTL(("access:etherStatsTable:ioctl", "_etherStats_ioctl_get\n"));
    /*
     * sanity checks
     */
    if (NULL == name) {
        DEBUGMSGTL(("access:etherStatsTable:ioctl",
                    "_etherStats_ioctl_get invalid ifname '%s'\n", name));
        snmp_log(LOG_ERR, "access:etherStatsTable:ioctl, _etherStats_ioctl_get error on inerface '%s'\n", name);
        return -1;
    }

    /*
     * create socket for ioctls
     */
    if (fd < 0) {
        fd = ourfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (ourfd < 0) {
            DEBUGMSGTL(("access:etherStatsTable:ioctl",
                        "_etherStats_ioctl_get couldn't create a socket\n"));
            snmp_log(LOG_ERR, "access:etherStatsTable:ioctl, _etherStats_ioctl_get error on inerface '%s'\n", name);

            return -2;
        }
    }

    strlcpy(ifrq->ifr_name, name, sizeof(ifrq->ifr_name));
    rc = ioctl(fd, which, ifrq);
    if (rc < 0) {
        DEBUGMSGTL(("access:etherStatsTable:ioctl",
                    "_etherStats_ioctl_get ioctl %d returned %d\n", which, rc));
        rc = -3;
    }

    if (ourfd >= 0) {
        close(ourfd);
    }

    return rc;
}


/**
 * load initial data
 *
 * TODO:350:M: Implement etherStatsTable data load
 * This function will also be called by the cache helper to load
 * the container again (after the container free function has been
 * called to free the previous contents).
 *
 * @param container container to which items should be inserted
 *
 * @retval MFD_SUCCESS              : success.
 * @retval MFD_RESOURCE_UNAVAILABLE : Can't access data source
 * @retval MFD_ERROR                : other error.
 *
 *  This function is called to load the index(es) (and data, optionally)
 *  for the every row in the data set.
 *
 * @remark
 *  While loading the data, the only important thing is the indexes.
 *  If access to your data is cheap/fast (e.g. you have a pointer to a
 *  structure in memory), it would make sense to update the data here.
 *  If, however, the accessing the data invovles more work (e.g. parsing
 *  some other existing data, or peforming calculations to derive the data),
 *  then you can limit yourself to setting the indexes and saving any
 *  information you will need later. Then use the saved information in
 *  etherStatsTable_row_prep() for populating data.
 *
 * @note
 *  If you need consistency between rows (like you want statistics
 *  for each row to be from the same time frame), you should set all
 *  data here.
 *
 */
int
etherStatsTable_entry_load(ETH_STATS_T * data, long ifIndex) {

    DEBUGMSGTL(("verbose:etherStatsTable:etherStatsTable_entry_load",
                "called\n"));

    /*
     * temporary storage for index values
     * etherStatsIndex(1)/INTEGER32/ASN_INTEGER/long(long)//l/A/w/e/R/d/h
     */
    long            etherStatsIndex;
    int             fd;
    int             rc = 0, retval = 0;
    struct ifname *list_head = NULL, *p = NULL;
    size_t          count = 0;

    memset(data, 0, sizeof(*data));

    /*
     * create socket for ioctls
     */
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        snmp_log(LOG_ERR, "could not create socket\n");
        return -2;
    }

    /*
     * get the interface names of the devices present in the system, in case of
     * failure retval suggests the reson for failure and list_head contains
     * null
     */
    list_head = etherstats_interface_name_list_get(list_head, &retval);
    if (!list_head) {
        snmp_log(LOG_ERR, "access:etherStatsTable, error getting the interface names present in the system\n");
        DEBUGMSGTL(("access:etherStatsTable", "error getting the interface names present in the system"));
        close(fd);
        return MFD_ERROR;
    }

    /*
     * Walk over the list of interface names present in the system and retreive
     * the statistics
     */

    for (p = list_head; p; p = p->ifn_next) {
        /*
         * get index via ioctl.
         */
        etherStatsIndex = (long) etherstats_interface_ioctl_ifindex_get(fd, p->name);
        DEBUGMSGTL(("access:etherStatsTable", "processing '%s' (index=%ld)\n", p->name, etherStatsIndex));
        if (etherStatsIndex != ifIndex) {
            continue;
        }

        /*
         *  get the etherstats contents populated, if the device is not an
         *  ethernet device the operation will not be supported and an error
         *  message will be logged
         */
        rc = interface_ioctl_etherstats_get(data, fd, p->name);
        if (rc < 0) {
            DEBUGMSGTL(("access:etherStatsTable", "error getting the statistics for interface |%s| "
                        "etherStatsTable data, operation might not be supported\n", p->name));
            continue;
        }

        ++count;
    }

    close(fd);

    /*
     * free the interface names list
     */
    if ((etherstats_interface_name_list_free(list_head)) < 0) {
        snmp_log(LOG_ERR, "access:etherStatsTable, error freeing the interface name list \n");
        DEBUGMSGTL(("access:etherStatsTable", "error freeing the interface name list\n"));
        return MFD_ERROR;
    }

    DEBUGMSGT(("verbose:etherStatsTable:etherStatsTable_entry_load",
               "inserted %" NETSNMP_PRIz "d records\n", count));

    return MFD_SUCCESS;
}


