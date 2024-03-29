/*
 * Note: this file originally auto-generated by mib2c using
 *       version : 15899 $ of $ 
 *
 * $Id:ipv6ScopeZoneIndexTable_interface.c 14170 2007-04-29 00:12:32Z varun_c$
 */
/*
 * *********************************************************************
 * *********************************************************************
 * *********************************************************************
 * ***                                                               ***
 * ***  NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE  ***
 * ***                                                               ***
 * ***                                                               ***
 * ***       THIS FILE DOES NOT CONTAIN ANY USER EDITABLE CODE.      ***
 * ***                                                               ***
 * ***                                                               ***
 * ***       THE GENERATED CODE IS INTERNAL IMPLEMENTATION, AND      ***
 * ***                                                               ***
 * ***                                                               ***
 * ***    IS SUBJECT TO CHANGE WITHOUT WARNING IN FUTURE RELEASES.   ***
 * ***                                                               ***
 * ***                                                               ***
 * *********************************************************************
 * *********************************************************************
 * *********************************************************************
 */

/*
 * standard Net-SNMP includes 
 */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-features.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/*
 * include our parent header 
 */
#include "ipv6ScopeZoneIndexTable.h"


#include <net-snmp/agent/table_container.h>
#include <net-snmp/library/container.h>

#include "ipv6ScopeZoneIndexTable_interface.h"

#include <ctype.h>

netsnmp_feature_child_of(ipv6ScopeZoneIndexTable_external_access, libnetsnmpmibs);

netsnmp_feature_require(row_merge);
netsnmp_feature_require(baby_steps);
netsnmp_feature_require(table_container_row_insert);
netsnmp_feature_require(check_all_requests_error);


netsnmp_feature_child_of(ipv6ScopeZoneIndexTable_container_size, ipv6ScopeZoneIndexTable_external_access);
netsnmp_feature_child_of(ipv6ScopeZoneIndexTable_registration_set, ipv6ScopeZoneIndexTable_external_access);
netsnmp_feature_child_of(ipv6ScopeZoneIndexTable_registration_get, ipv6ScopeZoneIndexTable_external_access);
netsnmp_feature_child_of(ipv6ScopeZoneIndexTable_container_get, ipv6ScopeZoneIndexTable_external_access);

/**********************************************************************
 **********************************************************************
 ***
 *** Table ipv6ScopeZoneIndexTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * IP-MIB::ipv6ScopeZoneIndexTable is subid 36 of ip.
 * Its status is Current.
 * OID: .1.3.6.1.2.1.4.36, length: 8
 */
typedef struct ipv6ScopeZoneIndexTable_interface_ctx_s {

    netsnmp_container *container;
    netsnmp_cache  *cache;
    ipv6ScopeZoneIndexTable_registration *user_ctx;

    netsnmp_table_registration_info tbl_info;

    netsnmp_baby_steps_access_methods access_multiplexer;

} ipv6ScopeZoneIndexTable_interface_ctx;

static ipv6ScopeZoneIndexTable_interface_ctx
    ipv6ScopeZoneIndexTable_if_ctx;

static void    
_ipv6ScopeZoneIndexTable_container_init
(ipv6ScopeZoneIndexTable_interface_ctx * if_ctx);
static void    
_ipv6ScopeZoneIndexTable_container_shutdown
(ipv6ScopeZoneIndexTable_interface_ctx * if_ctx);
static int
_cache_load(netsnmp_cache * cache, void *vmagic);
static void
_cache_free(netsnmp_cache * cache, void *magic);

#ifndef NETSNMP_FEATURE_REMOVE_IPV6SCOPEZONEINDEXTABLE_CONTAINER_GET
netsnmp_container *
ipv6ScopeZoneIndexTable_container_get(void)
{
    return ipv6ScopeZoneIndexTable_if_ctx.container;
}
#endif /* NETSNMP_FEATURE_REMOVE_IPV6SCOPEZONEINDEXTABLE_CONTAINER_GET */

#ifndef NETSNMP_FEATURE_REMOVE_IPV6SCOPEZONEINDEXTABLE_REGISTRATION_GET
ipv6ScopeZoneIndexTable_registration *
ipv6ScopeZoneIndexTable_registration_get(void)
{
    return ipv6ScopeZoneIndexTable_if_ctx.user_ctx;
}
#endif /* NETSNMP_FEATURE_REMOVE_IPV6SCOPEZONEINDEXTABLE_REGISTRATION_GET */

#ifndef NETSNMP_FEATURE_REMOVE_IPV6SCOPEZONEINDEXTABLE_REGISTRATION_SET
ipv6ScopeZoneIndexTable_registration *
ipv6ScopeZoneIndexTable_registration_set
    (ipv6ScopeZoneIndexTable_registration * newreg)
{
    ipv6ScopeZoneIndexTable_registration *old =
        ipv6ScopeZoneIndexTable_if_ctx.user_ctx;
    ipv6ScopeZoneIndexTable_if_ctx.user_ctx = newreg;
    return old;
}
#endif /* NETSNMP_FEATURE_REMOVE_IPV6SCOPEZONEINDEXTABLE_REGISTRATION_SET */

#ifndef NETSNMP_FEATURE_REMOVE_IPV6SCOPEZONEINDEXTABLE_CONTAINER_SIZE
int
ipv6ScopeZoneIndexTable_container_size(void)
{
    return CONTAINER_SIZE(ipv6ScopeZoneIndexTable_if_ctx.container);
}
#endif /* NETSNMP_FEATURE_REMOVE_IPV6SCOPEZONEINDEXTABLE_CONTAINER_SIZE */

/*
 * mfd multiplexer modes
 */
static Netsnmp_Node_Handler _mfd_ipv6ScopeZoneIndexTable_pre_request;
static Netsnmp_Node_Handler _mfd_ipv6ScopeZoneIndexTable_post_request;
static Netsnmp_Node_Handler _mfd_ipv6ScopeZoneIndexTable_object_lookup;
static Netsnmp_Node_Handler _mfd_ipv6ScopeZoneIndexTable_get_values;
/**
 * @internal
 * Initialize the table ipv6ScopeZoneIndexTable 
 *    (Define its contents and how it's structured)
 */
void
_ipv6ScopeZoneIndexTable_initialize_interface
    (ipv6ScopeZoneIndexTable_registration * reg_ptr, u_long flags)
{
    netsnmp_baby_steps_access_methods *access_multiplexer =
        &ipv6ScopeZoneIndexTable_if_ctx.access_multiplexer;
    netsnmp_table_registration_info *tbl_info =
        &ipv6ScopeZoneIndexTable_if_ctx.tbl_info;
    netsnmp_handler_registration *reginfo;
    netsnmp_mib_handler *handler;
    int             mfd_modes = 0;

    DEBUGMSGTL(("internal:ipv6ScopeZoneIndexTable:_ipv6ScopeZoneIndexTable_initialize_interface", "called\n"));


    /*************************************************
     *
     * save interface context for ipv6ScopeZoneIndexTable
     */
    /*
     * Setting up the table's definition
     */
    netsnmp_table_helper_add_indexes(tbl_info, ASN_INTEGER,
                                               /** index: ipv6ScopeZoneIndexIfIndex */
                                     0);

    /*
     * Define the minimum and maximum accessible columns.  This
     * optimizes retrieval. 
     */
    tbl_info->min_column = IPV6SCOPEZONEINDEXTABLE_MIN_COL;
    tbl_info->max_column = IPV6SCOPEZONEINDEXTABLE_MAX_COL;

    /*
     * save users context
     */
    ipv6ScopeZoneIndexTable_if_ctx.user_ctx = reg_ptr;

    /*
     * call data access initialization code
     */
    ipv6ScopeZoneIndexTable_init_data(reg_ptr);

    /*
     * set up the container
     */
    _ipv6ScopeZoneIndexTable_container_init
        (&ipv6ScopeZoneIndexTable_if_ctx);
    if (NULL == ipv6ScopeZoneIndexTable_if_ctx.container) {
        snmp_log(LOG_ERR,
                 "could not initialize container for ipv6ScopeZoneIndexTable\n");
        return;
    }

    /*
     * access_multiplexer: REQUIRED wrapper for get request handling
     */
    access_multiplexer->object_lookup =
        _mfd_ipv6ScopeZoneIndexTable_object_lookup;
    access_multiplexer->get_values =
        _mfd_ipv6ScopeZoneIndexTable_get_values;

    /*
     * no wrappers yet
     */
    access_multiplexer->pre_request =
        _mfd_ipv6ScopeZoneIndexTable_pre_request;
    access_multiplexer->post_request =
        _mfd_ipv6ScopeZoneIndexTable_post_request;


    /*************************************************
     *
     * Create a registration, save our reg data, register table.
     */
    DEBUGMSGTL(("ipv6ScopeZoneIndexTable:init_ipv6ScopeZoneIndexTable",
                "Registering ipv6ScopeZoneIndexTable as a mibs-for-dummies table.\n"));
    handler =
        netsnmp_baby_steps_access_multiplexer_get(access_multiplexer);
    reginfo =
        netsnmp_handler_registration_create("ipv6ScopeZoneIndexTable",
                                            handler,
                                            ipv6ScopeZoneIndexTable_oid,
                                            ipv6ScopeZoneIndexTable_oid_size,
                                            HANDLER_CAN_BABY_STEP |
                                            HANDLER_CAN_RONLY);
    if (NULL == reginfo) {
        snmp_log(LOG_ERR,
                 "error registering table ipv6ScopeZoneIndexTable\n");
        return;
    }
    reginfo->my_reg_void = &ipv6ScopeZoneIndexTable_if_ctx;

    /*************************************************
     *
     * set up baby steps handler, create it and inject it
     */
    if (access_multiplexer->object_lookup)
        mfd_modes |= BABY_STEP_OBJECT_LOOKUP;

    if (access_multiplexer->pre_request)
        mfd_modes |= BABY_STEP_PRE_REQUEST;
    if (access_multiplexer->post_request)
        mfd_modes |= BABY_STEP_POST_REQUEST;

#ifndef NETSNMP_NO_WRITE_SUPPORT
    if (access_multiplexer->set_values)
        mfd_modes |= BABY_STEP_SET_VALUES;
    if (access_multiplexer->irreversible_commit)
        mfd_modes |= BABY_STEP_IRREVERSIBLE_COMMIT;
    if (access_multiplexer->object_syntax_checks)
        mfd_modes |= BABY_STEP_CHECK_OBJECT;

    if (access_multiplexer->undo_setup)
        mfd_modes |= BABY_STEP_UNDO_SETUP;
    if (access_multiplexer->undo_cleanup)
        mfd_modes |= BABY_STEP_UNDO_CLEANUP;
    if (access_multiplexer->undo_sets)
        mfd_modes |= BABY_STEP_UNDO_SETS;

    if (access_multiplexer->row_creation)
        mfd_modes |= BABY_STEP_ROW_CREATE;
    if (access_multiplexer->consistency_checks)
        mfd_modes |= BABY_STEP_CHECK_CONSISTENCY;
    if (access_multiplexer->commit)
        mfd_modes |= BABY_STEP_COMMIT;
    if (access_multiplexer->undo_commit)
        mfd_modes |= BABY_STEP_UNDO_COMMIT;
#endif /* !NETSNMP_NO_WRITE_SUPPORT */

    handler = netsnmp_baby_steps_handler_get(mfd_modes);
    netsnmp_inject_handler(reginfo, handler);

    /*************************************************
     *
     * inject row_merge helper with prefix rootoid_len + 2 (entry.col)
     */
    handler = netsnmp_get_row_merge_handler(reginfo->rootoid_len + 2);
    netsnmp_inject_handler(reginfo, handler);

    /*************************************************
     *
     * inject container_table helper
     */
    handler =
        netsnmp_container_table_handler_get(tbl_info,
                                            ipv6ScopeZoneIndexTable_if_ctx.
                                            container,
                                            TABLE_CONTAINER_KEY_NETSNMP_INDEX);
    netsnmp_inject_handler(reginfo, handler);
   
    if (NULL != ipv6ScopeZoneIndexTable_if_ctx.cache) {
        handler =
            netsnmp_cache_handler_get(ipv6ScopeZoneIndexTable_if_ctx.cache);
        netsnmp_inject_handler(reginfo, handler);
    }

    /*
     * register table
     */
    netsnmp_register_table(reginfo, tbl_info);

}                               /* _ipv6ScopeZoneIndexTable_initialize_interface */

/**
 * @internal
 * Shutdown the table ipv6ScopeZoneIndexTable
 */
void
_ipv6ScopeZoneIndexTable_shutdown_interface
    (ipv6ScopeZoneIndexTable_registration * reg_ptr)
{
    /*
     * shutdown the container
     */
    _ipv6ScopeZoneIndexTable_container_shutdown
        (&ipv6ScopeZoneIndexTable_if_ctx);
}

void
ipv6ScopeZoneIndexTable_valid_columns_set(netsnmp_column_info *vc)
{
    ipv6ScopeZoneIndexTable_if_ctx.tbl_info.valid_columns = vc;
}                               /* ipv6ScopeZoneIndexTable_valid_columns_set */

/**
 * @internal
 * convert the index component stored in the context to an oid
 */
int
ipv6ScopeZoneIndexTable_index_to_oid(netsnmp_index * oid_idx,
                                     ipv6ScopeZoneIndexTable_mib_index *
                                     mib_idx)
{
    int             err = SNMP_ERR_NOERROR;

    /*
     * temp storage for parsing indexes
     */
    /*
     * ipv6ScopeZoneIndexIfIndex(1)/InterfaceIndex/ASN_INTEGER/long(long)//l/a/w/e/R/d/H
     */
    netsnmp_variable_list var_ipv6ScopeZoneIndexIfIndex;

    /*
     * set up varbinds
     */
    memset(&var_ipv6ScopeZoneIndexIfIndex, 0x00,
           sizeof(var_ipv6ScopeZoneIndexIfIndex));
    var_ipv6ScopeZoneIndexIfIndex.type = ASN_INTEGER;

    /*
     * chain temp index varbinds together
     */
    var_ipv6ScopeZoneIndexIfIndex.next_variable = NULL;


    DEBUGMSGTL(("verbose:ipv6ScopeZoneIndexTable:ipv6ScopeZoneIndexTable_index_to_oid", "called\n"));

    /*
     * ipv6ScopeZoneIndexIfIndex(1)/InterfaceIndex/ASN_INTEGER/long(long)//l/a/w/e/R/d/H 
     */
    snmp_set_var_value(&var_ipv6ScopeZoneIndexIfIndex,
                       (u_char *) & mib_idx->ipv6ScopeZoneIndexIfIndex,
                       sizeof(mib_idx->ipv6ScopeZoneIndexIfIndex));


    err = build_oid_noalloc(oid_idx->oids, oid_idx->len, &oid_idx->len,
                            NULL, 0, &var_ipv6ScopeZoneIndexIfIndex);
    if (err)
        snmp_log(LOG_ERR, "error %d converting index to oid\n", err);

    /*
     * parsing may have allocated memory. free it.
     */
    snmp_reset_var_buffers(&var_ipv6ScopeZoneIndexIfIndex);

    return err;
}                               /* ipv6ScopeZoneIndexTable_index_to_oid */

/**
 * extract ipv6ScopeZoneIndexTable indexes from a netsnmp_index
 *
 * @retval SNMP_ERR_NOERROR  : no error
 * @retval SNMP_ERR_GENERR   : error
 */
int
ipv6ScopeZoneIndexTable_index_from_oid(netsnmp_index * oid_idx,
                                       ipv6ScopeZoneIndexTable_mib_index *
                                       mib_idx)
{
    int             err = SNMP_ERR_NOERROR;

    /*
     * temp storage for parsing indexes
     */
    /*
     * ipv6ScopeZoneIndexIfIndex(1)/InterfaceIndex/ASN_INTEGER/long(long)//l/a/w/e/R/d/H
     */
    netsnmp_variable_list var_ipv6ScopeZoneIndexIfIndex;

    /*
     * set up varbinds
     */
    memset(&var_ipv6ScopeZoneIndexIfIndex, 0x00,
           sizeof(var_ipv6ScopeZoneIndexIfIndex));
    var_ipv6ScopeZoneIndexIfIndex.type = ASN_INTEGER;

    /*
     * chain temp index varbinds together
     */
    var_ipv6ScopeZoneIndexIfIndex.next_variable = NULL;


    DEBUGMSGTL(("verbose:ipv6ScopeZoneIndexTable:ipv6ScopeZoneIndexTable_index_from_oid", "called\n"));

    /*
     * parse the oid into the individual index components
     */
    err = parse_oid_indexes(oid_idx->oids, oid_idx->len,
                            &var_ipv6ScopeZoneIndexIfIndex);
    if (err == SNMP_ERR_NOERROR) {
        /*
         * copy out values
         */
        mib_idx->ipv6ScopeZoneIndexIfIndex =
            *((long *) var_ipv6ScopeZoneIndexIfIndex.val.string);


    }

    /*
     * parsing may have allocated memory. free it.
     */
    snmp_reset_var_buffers(&var_ipv6ScopeZoneIndexIfIndex);

    return err;
}                               /* ipv6ScopeZoneIndexTable_index_from_oid */


/*
 *********************************************************************
 * @internal
 * allocate resources for a ipv6ScopeZoneIndexTable_rowreq_ctx
 */
ipv6ScopeZoneIndexTable_rowreq_ctx *
ipv6ScopeZoneIndexTable_allocate_rowreq_ctx(ipv6ScopeZoneIndexTable_data *data,
                                             void *user_init_ctx)
{
    ipv6ScopeZoneIndexTable_rowreq_ctx *rowreq_ctx =
        SNMP_MALLOC_TYPEDEF(ipv6ScopeZoneIndexTable_rowreq_ctx);
    DEBUGMSGTL(("internal:ipv6ScopeZoneIndexTable:ipv6ScopeZoneIndexTable_allocate_rowreq_ctx", "called\n"));

    if (NULL == rowreq_ctx) {
        snmp_log(LOG_ERR, "Couldn't allocate memory for a "
                 "ipv6ScopeZoneIndexTable_rowreq_ctx.\n");
        return NULL;
    } else {
        if (NULL != data) {
            /*
             * track if we got data from user
             */
            rowreq_ctx->rowreq_flags |= MFD_ROW_DATA_FROM_USER;
            rowreq_ctx->data = data;
        } else if (NULL ==
                   (rowreq_ctx->data =
                    ipv6ScopeZoneIndexTable_allocate_data())) {
            SNMP_FREE(rowreq_ctx);
            return NULL;
        }
    }


    rowreq_ctx->oid_idx.oids = rowreq_ctx->oid_tmp;

    rowreq_ctx->ipv6ScopeZoneIndexTable_data_list = NULL;

    /*
     * if we allocated data, call init routine
     */
    if (!(rowreq_ctx->rowreq_flags & MFD_ROW_DATA_FROM_USER)) {
        if (SNMPERR_SUCCESS !=
            ipv6ScopeZoneIndexTable_rowreq_ctx_init(rowreq_ctx,
                                                    user_init_ctx)) {
            ipv6ScopeZoneIndexTable_release_rowreq_ctx(rowreq_ctx);
            rowreq_ctx = NULL;
        }
    }

    return rowreq_ctx;
}                               /* ipv6ScopeZoneIndexTable_allocate_rowreq_ctx */

/*
 * @internal
 * release resources for a ipv6ScopeZoneIndexTable_rowreq_ctx
 */
void
ipv6ScopeZoneIndexTable_release_rowreq_ctx
    (ipv6ScopeZoneIndexTable_rowreq_ctx * rowreq_ctx)
{
    DEBUGMSGTL(("internal:ipv6ScopeZoneIndexTable:ipv6ScopeZoneIndexTable_release_rowreq_ctx", "called\n"));

    netsnmp_assert(NULL != rowreq_ctx);

    ipv6ScopeZoneIndexTable_rowreq_ctx_cleanup(rowreq_ctx);
    /*
     * for non-transient data, don't free data we got from the user
     */
    if ((rowreq_ctx->data) &&
        !(rowreq_ctx->rowreq_flags & MFD_ROW_DATA_FROM_USER))
        ipv6ScopeZoneIndexTable_release_data(rowreq_ctx->data);


    /*
     * free index oid pointer
     */
    if (rowreq_ctx->oid_idx.oids != rowreq_ctx->oid_tmp)
        free(rowreq_ctx->oid_idx.oids);

    SNMP_FREE(rowreq_ctx);
}                               /* ipv6ScopeZoneIndexTable_release_rowreq_ctx */

/**
 * @internal
 * wrapper
 */
static int
_mfd_ipv6ScopeZoneIndexTable_pre_request(netsnmp_mib_handler *handler,
                                         netsnmp_handler_registration
                                         *reginfo,
                                         netsnmp_agent_request_info
                                         *agtreq_info,
                                         netsnmp_request_info *requests)
{
    int             rc;

    DEBUGMSGTL(("internal:ipv6ScopeZoneIndexTable:_mfd_ipv6ScopeZoneIndexTable_pre_request", "called\n"));

    if (1 != netsnmp_row_merge_status_first(reginfo, agtreq_info)) {
        DEBUGMSGTL(("internal:ipv6ScopeZoneIndexTable",
                    "skipping additional pre_request\n"));
        return SNMP_ERR_NOERROR;
    }

    rc = ipv6ScopeZoneIndexTable_pre_request
        (ipv6ScopeZoneIndexTable_if_ctx.user_ctx);
    if (MFD_SUCCESS != rc) {
        /*
         * nothing we can do about it but log it
         */
        DEBUGMSGTL(("ipv6ScopeZoneIndexTable", "error %d from "
                    "ipv6ScopeZoneIndexTable_pre_request\n", rc));
        netsnmp_request_set_error_all(requests, SNMP_VALIDATE_ERR(rc));
    }

    return SNMP_ERR_NOERROR;
}                               /* _mfd_ipv6ScopeZoneIndexTable_pre_request */

/**
 * @internal
 * wrapper
 */
static int
_mfd_ipv6ScopeZoneIndexTable_post_request(netsnmp_mib_handler *handler,
                                          netsnmp_handler_registration
                                          *reginfo,
                                          netsnmp_agent_request_info
                                          *agtreq_info,
                                          netsnmp_request_info *requests)
{
    ipv6ScopeZoneIndexTable_rowreq_ctx *rowreq_ctx = (ipv6ScopeZoneIndexTable_rowreq_ctx*)
        netsnmp_container_table_row_extract(requests);
    int             rc, packet_rc;

    DEBUGMSGTL(("internal:ipv6ScopeZoneIndexTable:_mfd_ipv6ScopeZoneIndexTable_post_request", "called\n"));

    /*
     * release row context, if deleted
     */
    if (rowreq_ctx && (rowreq_ctx->rowreq_flags & MFD_ROW_DELETED))
        ipv6ScopeZoneIndexTable_release_rowreq_ctx(rowreq_ctx);

    /*
     * wait for last call before calling user
     */
    if (1 != netsnmp_row_merge_status_last(reginfo, agtreq_info)) {
        DEBUGMSGTL(("internal:ipv6ScopeZoneIndexTable",
                    "waiting for last post_request\n"));
        return SNMP_ERR_NOERROR;
    }

    packet_rc = netsnmp_check_all_requests_error(agtreq_info->asp, 0);
    rc = ipv6ScopeZoneIndexTable_post_request
        (ipv6ScopeZoneIndexTable_if_ctx.user_ctx, packet_rc);
    if (MFD_SUCCESS != rc) {
        /*
         * nothing we can do about it but log it
         */
        DEBUGMSGTL(("ipv6ScopeZoneIndexTable", "error %d from "
                    "ipv6ScopeZoneIndexTable_post_request\n", rc));
    }

    return SNMP_ERR_NOERROR;
}                               /* _mfd_ipv6ScopeZoneIndexTable_post_request */

/**
 * @internal
 * wrapper
 */
static ipv6ScopeZoneIndexTable_rowreq_ctx *
_mfd_ipv6ScopeZoneIndexTable_rowreq_from_index(netsnmp_index * oid_idx,
                                           int *rc_ptr)
{
    ipv6ScopeZoneIndexTable_rowreq_ctx *rowreq_ctx;
    ipv6ScopeZoneIndexTable_mib_index mib_idx;
    int             rc;

    DEBUGMSGTL(("internal:ipv6ScopeZoneIndexTable:_mfd_ipv6ScopeZoneIndexTable_rowreq_from_index", "called\n"));

    if (NULL == rc_ptr)
        rc_ptr = &rc;
    *rc_ptr = MFD_SUCCESS;

    memset(&mib_idx, 0x0, sizeof(mib_idx));

    /*
     * try to parse oid
     */
    *rc_ptr = ipv6ScopeZoneIndexTable_index_from_oid(oid_idx, &mib_idx);
    if (MFD_SUCCESS != *rc_ptr) {
        DEBUGMSGT(("ipv6ScopeZoneIndexTable", "error parsing index\n"));
        return NULL;
    }

    /*
     * allocate new context
     */
    rowreq_ctx = ipv6ScopeZoneIndexTable_allocate_rowreq_ctx(NULL, NULL);
    if (NULL == rowreq_ctx) {
        *rc_ptr = MFD_ERROR;
        return NULL;            /* msg already logged */
    }

    memcpy(&rowreq_ctx->tbl_idx, &mib_idx, sizeof(mib_idx));


    /*
     * copy indexes
     */
    rowreq_ctx->oid_idx.len = oid_idx->len;
    memcpy(rowreq_ctx->oid_idx.oids, oid_idx->oids,
           oid_idx->len * sizeof(oid));

    return rowreq_ctx;
}                               /* _mfd_ipv6ScopeZoneIndexTable_rowreq_from_index */

/**
 * @internal
 * wrapper
 */
static int
_mfd_ipv6ScopeZoneIndexTable_object_lookup(netsnmp_mib_handler *handler,
                                           netsnmp_handler_registration
                                           *reginfo,
                                           netsnmp_agent_request_info
                                           *agtreq_info,
                                           netsnmp_request_info *requests)
{
    int             rc = SNMP_ERR_NOERROR;
    ipv6ScopeZoneIndexTable_rowreq_ctx *rowreq_ctx = (ipv6ScopeZoneIndexTable_rowreq_ctx*)
        netsnmp_container_table_row_extract(requests);

    DEBUGMSGTL(("internal:ipv6ScopeZoneIndexTable:_mfd_ipv6ScopeZoneIndexTable_object_lookup", "called\n"));

    /*
     * get our context from mfd
     * ipv6ScopeZoneIndexTable_interface_ctx *if_ctx =
     *             (ipv6ScopeZoneIndexTable_interface_ctx *)reginfo->my_reg_void;
     */

    if (NULL == rowreq_ctx) {
        netsnmp_table_request_info *tblreq_info;
        netsnmp_index   oid_idx;

        tblreq_info = netsnmp_extract_table_info(requests);
        if (NULL == tblreq_info) {
            snmp_log(LOG_ERR, "request had no table info\n");
            return MFD_ERROR;
        }

        /*
         * try create rowreq
         */
        oid_idx.oids = tblreq_info->index_oid;
        oid_idx.len = tblreq_info->index_oid_len;

        rowreq_ctx =
            _mfd_ipv6ScopeZoneIndexTable_rowreq_from_index(&oid_idx, &rc);
        if (MFD_SUCCESS == rc) {
            netsnmp_assert(NULL != rowreq_ctx);
            rowreq_ctx->rowreq_flags |= MFD_ROW_CREATED;
            /*
             * add rowreq_ctx to request data lists
             */
            netsnmp_container_table_row_insert(requests, (netsnmp_index *)
                                               rowreq_ctx);
        }

    }

    if (MFD_SUCCESS != rc)
        netsnmp_request_set_error_all(requests, rc);
    else
        ipv6ScopeZoneIndexTable_row_prep(rowreq_ctx);

    return SNMP_VALIDATE_ERR(rc);
}                               /* _mfd_ipv6ScopeZoneIndexTable_object_lookup */

/***********************************************************************
 *
 * GET processing
 *
 ***********************************************************************/
/*
 * @internal
 * Retrieve the value for a particular column
 */
NETSNMP_STATIC_INLINE int
_ipv6ScopeZoneIndexTable_get_column(ipv6ScopeZoneIndexTable_rowreq_ctx *
                                    rowreq_ctx,
                                    netsnmp_variable_list * var,
                                    int column)
{
    int             rc = SNMPERR_SUCCESS;

    DEBUGMSGTL(("internal:ipv6ScopeZoneIndexTable:_mfd_ipv6ScopeZoneIndexTable_get_column", "called for %d\n", column));


    netsnmp_assert(NULL != rowreq_ctx);

    switch (column) {

        /*
         * ipv6ScopeZoneIndexLinkLocal(2)/InetZoneIndex/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/r/d/H 
         */
    case COLUMN_IPV6SCOPEZONEINDEXLINKLOCAL:
        var->val_len = sizeof(u_long);
        var->type = ASN_UNSIGNED;
        rc = ipv6ScopeZoneIndexLinkLocal_get(rowreq_ctx,
                                             (u_long *) var->val.string);
        break;

        /*
         * ipv6ScopeZoneIndex3(3)/InetZoneIndex/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/r/d/H 
         */
    case COLUMN_IPV6SCOPEZONEINDEX3:
        var->val_len = sizeof(u_long);
        var->type = ASN_UNSIGNED;
        rc = ipv6ScopeZoneIndex3_get(rowreq_ctx,
                                     (u_long *) var->val.string);
        break;

        /*
         * ipv6ScopeZoneIndexAdminLocal(4)/InetZoneIndex/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/r/d/H 
         */
    case COLUMN_IPV6SCOPEZONEINDEXADMINLOCAL:
        var->val_len = sizeof(u_long);
        var->type = ASN_UNSIGNED;
        rc = ipv6ScopeZoneIndexAdminLocal_get(rowreq_ctx,
                                              (u_long *) var->val.string);
        break;

        /*
         * ipv6ScopeZoneIndexSiteLocal(5)/InetZoneIndex/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/r/d/H 
         */
    case COLUMN_IPV6SCOPEZONEINDEXSITELOCAL:
        var->val_len = sizeof(u_long);
        var->type = ASN_UNSIGNED;
        rc = ipv6ScopeZoneIndexSiteLocal_get(rowreq_ctx,
                                             (u_long *) var->val.string);
        break;

        /*
         * ipv6ScopeZoneIndex6(6)/InetZoneIndex/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/r/d/H 
         */
    case COLUMN_IPV6SCOPEZONEINDEX6:
        var->val_len = sizeof(u_long);
        var->type = ASN_UNSIGNED;
        rc = ipv6ScopeZoneIndex6_get(rowreq_ctx,
                                     (u_long *) var->val.string);
        break;

        /*
         * ipv6ScopeZoneIndex7(7)/InetZoneIndex/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/r/d/H 
         */
    case COLUMN_IPV6SCOPEZONEINDEX7:
        var->val_len = sizeof(u_long);
        var->type = ASN_UNSIGNED;
        rc = ipv6ScopeZoneIndex7_get(rowreq_ctx,
                                     (u_long *) var->val.string);
        break;

        /*
         * ipv6ScopeZoneIndexOrganizationLocal(8)/InetZoneIndex/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/r/d/H 
         */
    case COLUMN_IPV6SCOPEZONEINDEXORGANIZATIONLOCAL:
        var->val_len = sizeof(u_long);
        var->type = ASN_UNSIGNED;
        rc = ipv6ScopeZoneIndexOrganizationLocal_get(rowreq_ctx,
                                                     (u_long *) var->val.
                                                     string);
        break;

        /*
         * ipv6ScopeZoneIndex9(9)/InetZoneIndex/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/r/d/H 
         */
    case COLUMN_IPV6SCOPEZONEINDEX9:
        var->val_len = sizeof(u_long);
        var->type = ASN_UNSIGNED;
        rc = ipv6ScopeZoneIndex9_get(rowreq_ctx,
                                     (u_long *) var->val.string);
        break;

        /*
         * ipv6ScopeZoneIndexA(10)/InetZoneIndex/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/r/d/H 
         */
    case COLUMN_IPV6SCOPEZONEINDEXA:
        var->val_len = sizeof(u_long);
        var->type = ASN_UNSIGNED;
        rc = ipv6ScopeZoneIndexA_get(rowreq_ctx,
                                     (u_long *) var->val.string);
        break;

        /*
         * ipv6ScopeZoneIndexB(11)/InetZoneIndex/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/r/d/H 
         */
    case COLUMN_IPV6SCOPEZONEINDEXB:
        var->val_len = sizeof(u_long);
        var->type = ASN_UNSIGNED;
        rc = ipv6ScopeZoneIndexB_get(rowreq_ctx,
                                     (u_long *) var->val.string);
        break;

        /*
         * ipv6ScopeZoneIndexC(12)/InetZoneIndex/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/r/d/H 
         */
    case COLUMN_IPV6SCOPEZONEINDEXC:
        var->val_len = sizeof(u_long);
        var->type = ASN_UNSIGNED;
        rc = ipv6ScopeZoneIndexC_get(rowreq_ctx,
                                     (u_long *) var->val.string);
        break;

        /*
         * ipv6ScopeZoneIndexD(13)/InetZoneIndex/ASN_UNSIGNED/u_long(u_long)//l/A/w/e/r/d/H 
         */
    case COLUMN_IPV6SCOPEZONEINDEXD:
        var->val_len = sizeof(u_long);
        var->type = ASN_UNSIGNED;
        rc = ipv6ScopeZoneIndexD_get(rowreq_ctx,
                                     (u_long *) var->val.string);
        break;

    default:
        if (IPV6SCOPEZONEINDEXTABLE_MIN_COL <= column
            && column <= IPV6SCOPEZONEINDEXTABLE_MAX_COL) {
            DEBUGMSGTL(("internal:ipv6ScopeZoneIndexTable:_mfd_ipv6ScopeZoneIndexTable_get_column", "assume column %d is reserved\n", column));
            rc = MFD_SKIP;
        } else {
            snmp_log(LOG_ERR,
                     "unknown column %d in _ipv6ScopeZoneIndexTable_get_column\n",
                     column);
        }
        break;
    }

    return rc;
}                               /* _ipv6ScopeZoneIndexTable_get_column */

int
_mfd_ipv6ScopeZoneIndexTable_get_values(netsnmp_mib_handler *handler,
                                        netsnmp_handler_registration
                                        *reginfo,
                                        netsnmp_agent_request_info
                                        *agtreq_info,
                                        netsnmp_request_info *requests)
{
    ipv6ScopeZoneIndexTable_rowreq_ctx *rowreq_ctx = (ipv6ScopeZoneIndexTable_rowreq_ctx*)
        netsnmp_container_table_row_extract(requests);
    netsnmp_table_request_info *tri;
    u_char         *old_string;
    void            (*dataFreeHook) (void *);
    int             rc;

    DEBUGMSGTL(("internal:ipv6ScopeZoneIndexTable:_mfd_ipv6ScopeZoneIndexTable_get_values", "called\n"));
    netsnmp_assert(NULL != rowreq_ctx);

    for (; requests; requests = requests->next) {
        /*
         * save old pointer, so we can free it if replaced
         */
        old_string = requests->requestvb->val.string;
        dataFreeHook = requests->requestvb->dataFreeHook;
        if (NULL == requests->requestvb->val.string) {
            requests->requestvb->val.string = requests->requestvb->buf;
            requests->requestvb->val_len =
                sizeof(requests->requestvb->buf);
        } else if (requests->requestvb->buf ==
                   requests->requestvb->val.string) {
            if (requests->requestvb->val_len !=
                sizeof(requests->requestvb->buf))
                requests->requestvb->val_len =
                    sizeof(requests->requestvb->buf);
        }

        /*
         * get column data
         */
        tri = netsnmp_extract_table_info(requests);
        if (NULL == tri)
            continue;

        rc = _ipv6ScopeZoneIndexTable_get_column(rowreq_ctx,
                                                 requests->requestvb,
                                                 tri->colnum);
        if (rc) {
            if (MFD_SKIP == rc) {
                requests->requestvb->type = SNMP_NOSUCHINSTANCE;
                rc = SNMP_ERR_NOERROR;
            }
        } else if (NULL == requests->requestvb->val.string) {
            snmp_log(LOG_ERR, "NULL varbind data pointer!\n");
            rc = SNMP_ERR_GENERR;
        }
        if (rc)
            netsnmp_request_set_error(requests, SNMP_VALIDATE_ERR(rc));

        /*
         * if the buffer wasn't used previously for the old data (i.e. it
         * was allocated memory)  and the get routine replaced the pointer,
         * we need to free the previous pointer.
         */
        if (old_string && (old_string != requests->requestvb->buf) &&
            (requests->requestvb->val.string != old_string)) {
            if (dataFreeHook)
                (*dataFreeHook) (old_string);
            else
                free(old_string);
        }
    }                           /* for results */

    return SNMP_ERR_NOERROR;
}                               /* _mfd_ipv6ScopeZoneIndexTable_get_values */


/***********************************************************************
 *
 * SET processing
 *
 ***********************************************************************/

/*
 * SET PROCESSING NOT APPLICABLE (per MIB or user setting)
 */
/***********************************************************************
 *
 * DATA ACCESS
 *
 ***********************************************************************/
/**
 * @internal
 */
static void
_container_item_free(ipv6ScopeZoneIndexTable_rowreq_ctx * rowreq_ctx,
                     void *context)
{
    DEBUGMSGTL(("internal:ipv6ScopeZoneIndexTable:_container_item_free",
                "called\n"));

    if (NULL == rowreq_ctx)
        return;

    ipv6ScopeZoneIndexTable_release_rowreq_ctx(rowreq_ctx);
}                               /* _container_item_free */

/**
 * @internal
 */
static void
_container_free(netsnmp_container * container)
{
    DEBUGMSGTL(("internal:ipv6ScopeZoneIndexTable:_container_free",
                "called\n"));

    if (NULL == container) {
        snmp_log(LOG_ERR,
                 "invalid container in ipv6ScopeZoneIndexTable_container_free\n");
        return;
    }

    /*
     * call user code
     */
    ipv6ScopeZoneIndexTable_container_free(container);

    /*
     * free all items. inefficient, but easy.
     */
    CONTAINER_CLEAR(container,
                    (netsnmp_container_obj_func *) _container_item_free,
                    NULL);
}                               /* _container_free */

/**
 * @internal
 * initialize the container with functions or wrappers
 */
void
_ipv6ScopeZoneIndexTable_container_init
    (ipv6ScopeZoneIndexTable_interface_ctx * if_ctx)
{
    DEBUGMSGTL(("internal:ipv6ScopeZoneIndexTable:_ipv6ScopeZoneIndexTable_container_init", "called\n"));

    /*
     * container init
     */
    if_ctx->cache = netsnmp_cache_create(30,    /* timeout in seconds */
                                         _cache_load, _cache_free,
                                         ipv6ScopeZoneIndexTable_oid,
                                         ipv6ScopeZoneIndexTable_oid_size);

    if (NULL == if_ctx->cache) {
        snmp_log(LOG_ERR, "error creating cache for ipScopeZoneIndexTable\n");
        return;
    }

    if_ctx->cache->flags = NETSNMP_CACHE_DONT_INVALIDATE_ON_SET;

    ipv6ScopeZoneIndexTable_container_init(&if_ctx->container, if_ctx->cache);
    if (NULL == if_ctx->container) {
        if_ctx->container =
            netsnmp_container_find
            ("ipv6ScopeZoneIndexTable:table_container");
        if (if_ctx->container)
            if_ctx->container->container_name =
                 strdup("ipv6ScopeZoneIndexTable");
    }
    if (NULL == if_ctx->container) {
        snmp_log(LOG_ERR, "error creating container in "
                 "ipv6ScopeZoneIndexTable_container_init\n");
        return;
    }
    if (NULL != if_ctx->cache)
        if_ctx->cache->magic = (void *) if_ctx->container;


}                               /* _ipv6ScopeZoneIndexTable_container_init */

/**
 * @internal
 * shutdown the container with functions or wrappers
 */
void
_ipv6ScopeZoneIndexTable_container_shutdown
    (ipv6ScopeZoneIndexTable_interface_ctx * if_ctx)
{
    DEBUGMSGTL(("internal:ipv6ScopeZoneIndexTable:_ipv6ScopeZoneIndexTable_container_shutdown", "called\n"));

    ipv6ScopeZoneIndexTable_container_shutdown(if_ctx->container);

    _container_free(if_ctx->container);

}                               /* _ipv6ScopeZoneIndexTable_container_shutdown */


#ifndef NETSNMP_FEATURE_REMOVE_IPV6SCOPEZONEINDEXTABLE_EXTERNAL_ACCESS
ipv6ScopeZoneIndexTable_rowreq_ctx *
ipv6ScopeZoneIndexTable_row_find_by_mib_index
    (ipv6ScopeZoneIndexTable_mib_index * mib_idx)
{
    ipv6ScopeZoneIndexTable_rowreq_ctx *rowreq_ctx;
    oid             oid_tmp[MAX_OID_LEN];
    netsnmp_index   oid_idx;
    int             rc;

    /*
     * set up storage for OID
     */
    oid_idx.oids = oid_tmp;
    oid_idx.len = OID_LENGTH(oid_tmp);

    /*
     * convert
     */
    rc = ipv6ScopeZoneIndexTable_index_to_oid(&oid_idx, mib_idx);
    if (MFD_SUCCESS != rc)
        return NULL;

    rowreq_ctx = (ipv6ScopeZoneIndexTable_rowreq_ctx*)
        CONTAINER_FIND(ipv6ScopeZoneIndexTable_if_ctx.container, &oid_idx);

    return rowreq_ctx;
}
#endif /* NETSNMP_FEATURE_REMOVE_IPV6SCOPEZONEINDEXTABLE_EXTERNAL_ACCESS */

static int
_cache_load(netsnmp_cache * cache, void *vmagic)
{
    DEBUGMSGTL(("internal:ipScopeZoneIndexTable:_cache_load", "called\n"));

    if ((NULL == cache) || (NULL == cache->magic)) {
        snmp_log(LOG_ERR,
                 "invalid cache for ipScopeZoneIndexTable_cache_load\n");
        return -1;
    }
    /** should only be called for an invalid or expired cache */
    netsnmp_assert((0 == cache->valid) || (1 == cache->expired));

    /*
     * call user code
     */
    return ipv6ScopeZoneIndexTable_container_load((netsnmp_container *) cache->
                                             magic);
}                               /* _cache_load */

/**
 * @internal
 */
static void
_cache_free(netsnmp_cache * cache, void *magic)
{
    netsnmp_container *container;

    DEBUGMSGTL(("internal:ipScopeZoneIndexTable:_cache_free", "called\n"));

    if ((NULL == cache) || (NULL == cache->magic)) {
        snmp_log(LOG_ERR,
                 "invalid cache in ipScopeZoneIndexTable_cache_free\n");
        return;
    }

    container = (netsnmp_container *) cache->magic;

    _container_free(container);
}                               /* _cache_free */

