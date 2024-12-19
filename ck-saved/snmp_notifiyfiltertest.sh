#!/bin/sh

###FIXME SNMP_PORT=4700
###XXX SNMP_TARGET="defaultV2Trap"
SNMP_PORT=161
SNMP_TARGET="internal0"
SNMP_HOST=" -r0 localhost:${SNMP_PORT}"

set -x  # be verbose

##############################################################
# active(1) notInService(2) notReady(3) createAndGo(4) createAndWait(5) destroy(6)
##############################################################
# snmpset ${SNMP_HOST} snmpNotifyFilterProfileRowStatus.\'${SNMP_TARGET}\' = destroy
# snmpset ${SNMP_HOST} snmpNotifyFilterProfileName.\'${SNMP_TARGET}\' = svfua
#FIXME: Error in packet.
# Reason: (noSuchName) There is no such variable name in this MIB.
# Failed object: SNMP-NOTIFICATION-MIB::snmpNotifyFilterProfileName.\'internal0\'
############################# OK ############################


# snmpset ${SNMP_HOST} snmpNotifyFilterProfileRowStatus.\'${SNMP_TARGET}\' = destroy
# single shot:
# snmpset ${SNMP_HOST} snmpNotifyFilterProfileRowStatus.\'${SNMP_TARGET}\' = createAndGo \
#                      snmpNotifyFilterProfileName.\'${SNMP_TARGET}\' = svfua
# snmpget ${SNMP_HOST} snmpNotifyFilterProfileRowStatus.\'${SNMP_TARGET}\' #XXX---> = notReady
############################# OK ############################


# snmpset ${SNMP_HOST} snmpNotifyFilterProfileRowStatus.\'${SNMP_TARGET}\' = destroy
# snmpset ${SNMP_HOST} snmpNotifyFilterProfileRowStatus.\'${SNMP_TARGET}\' = createAndWait
# snmpset ${SNMP_HOST} snmpNotifyFilterProfileName.\'${SNMP_TARGET}\' = svfua \
#                      snmpNotifyFilterProfileRowStatus.\'${SNMP_TARGET}\' = active
#FIXME: Error in packet.
# Reason: inconsistentValue (The set value is illegal or unsupported in some way)
# Failed object: SNMP-NOTIFICATION-MIB::snmpNotifyFilterProfileRowStatus.\'internal0\'
############################# BUG ############################

# snmpset ${SNMP_HOST} snmpNotifyFilterProfileRowStatus.\'${SNMP_TARGET}\' = destroy
# snmpset ${SNMP_HOST} snmpNotifyFilterProfileRowStatus.\'${SNMP_TARGET}\' = createAndWait \
#                      snmpNotifyFilterProfileName.\'${SNMP_TARGET}\' = svfua \
#                      snmpNotifyFilterProfileRowStatus.\'${SNMP_TARGET}\' = notInService # notReady
#FIXME: Error in packet.
# Reason: (noSuchName) There is no such variable name in this MIB.
# Failed object: SNMP-NOTIFICATION-MIB::snmpNotifyFilterProfileName.\'internal0\'
############################# BUG ############################
snmpget ${SNMP_HOST} snmpNotifyFilterProfileRowStatus.\'${SNMP_TARGET}\' #XXX---> = notInService
snmpset ${SNMP_HOST} snmpNotifyFilterProfileRowStatus.\'${SNMP_TARGET}\' = notInService

snmpset ${SNMP_HOST} snmpNotifyFilterProfileRowStatus.\'${SNMP_TARGET}\' = destroy
# tipple mode
snmpset ${SNMP_HOST} snmpNotifyFilterProfileRowStatus.\'${SNMP_TARGET}\' = createAndGo
snmpset ${SNMP_HOST} snmpNotifyFilterProfileName.\'${SNMP_TARGET}\' = svfua
snmpget ${SNMP_HOST} snmpNotifyFilterProfileRowStatus.\'${SNMP_TARGET}\' #XXX---> = notReady
snmpset ${SNMP_HOST} snmpNotifyFilterProfileRowStatus.\'${SNMP_TARGET}\' = active
############################# OK #############################

######################
# snmptable ${SNMP_HOST} snmpNotifyFilterProfileTable
# exit
######################

set -e  # exit on error

###notWritable snmpset ${SNMP_HOST} nlmConfigLogEntryStatus.\"\"   = 2
# snmpset ${SNMP_HOST} nlmConfigLogFilterName.\"\"    = "svfua"
###notWritable snmpset ${SNMP_HOST} nlmConfigLogEntryStatus.\"\"   = active

# Note that when the value of this object is the zero-length string, this
# extension rule results in a mask of all-1's being used (i.e., no 'wild
# card'),
# 
### snmpset ${SNMP_HOST}  snmpNotifyFilterRowStatus.\"svfua\".1.3.6.1.4.1.59999 = destroy
### snmpset ${SNMP_HOST}  snmpNotifyFilterRowStatus.\"svfua\".1.3.6.1.4.1.59999 = createAndGo
# snmpset ${SNMP_HOST}  snmpNotifyFilterMask.\"svfua\".1.3.6.1.4.1.59999 = f0
# snmpset ${SNMP_HOST}  snmpNotifyFilterType.\"svfua\".1.3.6.1.4.1.59999 = included

##############################################################
# SVFUA-LOG-MIB::svfuaLogNotifyFull included!
##############################################################
snmpset ${SNMP_HOST}  snmpNotifyFilterRowStatus.\"svfua\".1.3.6.1.4.1.59999.33.1.3.2.0.2 = destroy
snmpset ${SNMP_HOST}  snmpNotifyFilterRowStatus.\"svfua\".1.3.6.1.4.1.59999.33.1.3.2.0.2 = createAndGo
###XXX snmpset ${SNMP_HOST}  snmpNotifyFilterType.\"svfua\".1.3.6.1.4.1.59999.33.1.3.2.0.2 = included

##############################################################
#SVFUA-LOG-MIB::svfuaLogMgmtMaxSize.securityLog NOT allowed!
##############################################################
snmpset ${SNMP_HOST}  snmpNotifyFilterRowStatus.\"svfua\".1.3.6.1.4.1.59999.33.1.3.1.2.1.2.2 = destroy
snmpset ${SNMP_HOST}  snmpNotifyFilterRowStatus.\"svfua\".1.3.6.1.4.1.59999.33.1.3.1.2.1.2.2 = createAndGo
snmpset ${SNMP_HOST}  snmpNotifyFilterMask.\"svfua\".1.3.6.1.4.1.59999.33.1.3.1.2.1.2.2 = fffb
snmpset ${SNMP_HOST}  snmpNotifyFilterType.\"svfua\".1.3.6.1.4.1.59999.33.1.3.1.2.1.2.2 = excluded

snmpset ${SNMP_HOST}  UCD-SNMP-MIB::versionSavePersistentData.0 = 1 || echo ignored

# cat /var/net-snmp/snmpd.conf
##############################################################
#
# snmpNotifyFilterTable persistent data
#
# snmpNotifyFilterProfileTable  "internal0" "svfua" 3 1
# snmpNotifyFilterTable .5.115.118.102.117.97.1.3.6.1.4.1.59999.33.1.3.1.2.1.2.2 2:"fffb" 3:2 4:3 5:1 $
# snmpNotifyFilterTable .5.115.118.102.117.97.1.3.6.1.4.1.59999.33.1.3.2.0.2 2:0x 3:1 4:3 5:1 $
##############################################################

MIBTABLES=" snmpTargetParamsTable snmpTargetAddrTable 
  snmpNotifyTable snmpNotifyFilterProfileTable snmpNotifyFilterTable
  nlmConfigLogTable nlmStatsLogTable nlmLogTable nlmLogVariableTable
"

#XXX MIBTABLES=" sysORTable usmUserTable vacmAccessTable vacmcontextTable vacmSecurityToGroupTable vacmViewTreeFamilyTable"

for Table in $MIBTABLES; do snmptable -r 0 -C biw 100 ${SNMP_HOST}  $Table; done

# SNMP table: SNMP-TARGET-MIB::snmpTargetParamsTable
# 
#         index MPModel SecurityModel SecurityName SecurityLevel StorageType RowStatus
# \'internal0\'       1             2      private  noAuthNoPriv    readOnly    active
# SNMP table: SNMP-TARGET-MIB::snmpTargetAddrTable
# 
#         index                  TDomain             TAddress Timeout RetryCount   TagList    Params
# \'internal0\' SNMPv2-TM::snmpUDPDomain "7F 00 00 01 00 A2 "    1000          5 internal0 internal0
# 
# SNMP table SNMP-TARGET-MIB::snmpTargetAddrTable, part 2
# 
#         index StorageType RowStatus
# \'internal0\'    readOnly    active
# SNMP table: SNMP-NOTIFICATION-MIB::snmpNotifyTable
# 
#         index       Tag Type StorageType RowStatus
# \'internal0\' internal0 trap    readOnly    active
# SNMP table: SNMP-NOTIFICATION-MIB::snmpNotifyFilterProfileTable
# 
#         index  Name    StorType RowStatus
# \'internal0\' svfua nonVolatile    active
# SNMP table: SNMP-NOTIFICATION-MIB::snmpNotifyFilterTable
# 
#                                        index   Mask     Type StorageType RowStatus
#                  \"svfua\".1.3.6.1.4.1.59999     "" included nonVolatile    active
# \"svfua\".1.3.6.1.4.1.59999.33.1.3.1.2.1.2.2 "fffb" excluded nonVolatile    active
# 
