/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.scalar.conf,v 1.8 2004/10/14 12:57:34 dts12 Exp $
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include <net-snmp/data_access/ip_scalars.h>

#include "ip_scalars.h"

int
handle_ipv6IpForwarding(netsnmp_mib_handler *handler,
                        netsnmp_handler_registration *reginfo,
                        netsnmp_agent_request_info *reqinfo,
                        netsnmp_request_info *requests);

/** Initializes the ip module */
void
init_ip_scalars(void)
{
    static oid      ipReasmTimeout_oid[] = { 1, 3, 6, 1, 2, 1, 4, 13, 0 };
    static oid      ipv6IpForwarding_oid[] = { 1, 3, 6, 1, 2, 1, 4, 25 };
    static oid      ipv6IpDefaultHopLimit_oid[] =
        { 1, 3, 6, 1, 2, 1, 4, 26, 0 };

    DEBUGMSGTL(("ip_scalar", "Initializing\n"));

    netsnmp_register_num_file_instance
        ("ipReasmTimeout",
         ipReasmTimeout_oid, OID_LENGTH(ipReasmTimeout_oid),
         "/proc/sys/net/ipv4/ipfrag_time", ASN_INTEGER,
         HANDLER_CAN_RONLY, NULL, NULL);
                                       
    netsnmp_register_scalar(netsnmp_create_handler_registration
                            ("ipv6IpForwarding", handle_ipv6IpForwarding,
                             ipv6IpForwarding_oid,
                             OID_LENGTH(ipv6IpForwarding_oid),
                             HANDLER_CAN_RWRITE));

    netsnmp_register_num_file_instance
        ("ipv6IpDefaultHopLimit",
         ipv6IpDefaultHopLimit_oid, OID_LENGTH(ipv6IpDefaultHopLimit_oid),
         "/proc/sys/net/ipv6/conf/default/hop_limit", ASN_INTEGER,
         HANDLER_CAN_RWRITE, NULL, NULL);
                                       
}

int
handle_ipv6IpForwarding(netsnmp_mib_handler *handler,
                        netsnmp_handler_registration *reginfo,
                        netsnmp_agent_request_info *reqinfo,
                        netsnmp_request_info *requests)
{
    int      rc;
    u_long   value;

    /*
     * We are never called for a GETNEXT if it's registered as a
     * "instance", as it's "magically" handled for us.  
     */

    /*
     * a instance handler also only hands us one request at a time, so
     * we don't need to loop over a list of requests; we'll only get one. 
     */
    switch (reqinfo->mode) {

    case MODE_GET:
        rc = netsnmp_arch_ip_scalars_ipv6IpForwarding_get(&value);
        if (rc != 0) {
            netsnmp_set_request_error(reqinfo, requests,
                                      SNMP_NOSUCHINSTANCE);
        }
        else {
            value = value ? 1 : 2;
            snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
                                     (u_char *)&value, sizeof(value));
        }
        break;

        /*
         * SET REQUEST
         *
         * multiple states in the transaction.  See:
         * http://www.net-snmp.org/tutorial-5/toolkit/mib_module/set-actions.jpg
         */
    case MODE_SET_RESERVE1:
        break;

    case MODE_SET_RESERVE2:
        /*
         * store old info for undo later 
         */
        rc = netsnmp_arch_ip_scalars_ipv6IpForwarding_get(&value);
        if (rc < 0) {
            netsnmp_set_request_error(reqinfo, requests,
                                      SNMP_ERR_NOCREATION);
        }
        else {
            u_long *value_save;
            memdup((u_char **) & value_save, (u_char *) &value,
                   sizeof(value));
            if ( NULL == value_save ) {
                netsnmp_set_request_error(reqinfo, requests,
                                          SNMP_ERR_RESOURCEUNAVAILABLE);
            }
            else {
                netsnmp_request_add_list_data(requests,
                                              netsnmp_create_data_list
                                              ("ipfw", value_save,
                                               free));
            }
        }
        break;

    case MODE_SET_FREE:
        break;

    case MODE_SET_ACTION:
        value =  *(requests->requestvb->val.integer);
        rc = netsnmp_arch_ip_scalars_ipv6IpForwarding_set(value);
        if ( 0 != rc ) {
            netsnmp_set_request_error(reqinfo, requests, rc );
        }
        break;

    case MODE_SET_COMMIT:
        break;

    case MODE_SET_UNDO:
        value =
            *((u_long *) netsnmp_request_get_list_data(requests,
                                                       "ipfw"));
        rc = netsnmp_arch_ip_scalars_ipv6IpForwarding_set(value);
        if ( 0 != rc ) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_UNDOFAILED);
        }
        break;

    default:
        /*
         * we should never get here, so this is a really bad error 
         */
        snmp_log(LOG_ERR, "unknown mode (%d) in handle_ipv6IpForwarding\n",
                 reqinfo->mode);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
