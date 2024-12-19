#!/bin/sh
#
# Author:  Stephan Schloesser, Claus Klein
# Date:    $Date: 2005/10/14 15:44:15 $
# Version: $Revision: 1.4 $
#
# Description:
#   Configures a new notification receiver
#
#   The IP address of the notification receiver and other SNMPv3 parameters
#   to use in generating notifications are added to the appropriate MIB
#   tables.
#
#   MIBs used/needed:
#   - SNMP-NOTIFICATION-MIB [RFC 3413]
#   - SNMP-TARGET-MIB [RFC 3413]
#

: ${HOST:="localhost"}
: ${HOST-="$1"}
shift

###FIXME SNMP_PORT=4700
SNMP_HOST="-r0 ${HOST}:${SNMP_PORT}"

### : ${SNMP_VERSION:=2c}
: ${SNMP_VERSION:="3 -u unsec -l noAuthNoPriv"}
### : ${SNMP_VERSION:="3 -u v3rwPriv -l authPriv -a MD5 -A snmpPriv -x DES -X snmpPriv"}
## : ${SNMP_COMMUNITY:="public"}
: ${TRAP_HOST:="NMSV6"}
#FIXME : ${TRAP_HOST:="10.11.12.13"}
: ${TRAP_COMMUNITY:="trapuser"}

SNMP_SET=snmpset
SNMP_GET=snmpget
SNMP_WALK=snmpwalk

set -e	# exit on error
set -u	# exit on undefind vars

# GetHostByName and append udp port number:
###XXX### ping -c 1 ${TRAP_HOST}
#XXX UDPDomain=$(nslookup ${TRAP_HOST} | awk 'BEGIN { Address = 0 }; /Address:/ { Address = $2 }; END { print Address }' | awk -F . ' {printf "%02x%02x%02x%02x%04x\n", $1, $2, $3, $4, 162 }')
#FIXME DomainUdpIpv6=$(nslookup ${TRAP_HOST} | awk 'BEGIN { Address = 0 }; /Address:/ { Address = $2 }; END { print Address }' | awk -F . ' {printf "00000000000000000000FFFF%02x%02x%02x%02x%04x\n", $1, $2, $3, $4, 162 }')
DomainUdpIpv6=$(ping -c1 ${TRAP_HOST} | awk 'BEGIN { Address = "127.0.0.1" }; /64 bytes from:/ { Address = $4 }; END { print Address }' | awk -F . ' {printf "00000000000000000000FFFF%02x%02x%02x%02x%04x\n", $1, $2, $3, $4, 162 }')
### DomainUdpIpv6=$(echo ${TRAP_HOST} | awk -F . ' {printf "00000000000000000000FFFF%02x%02x%02x%02x%04x\n", $1, $2, $3, $4, 162 }')
#
# NOTE: host is not available on AXR! ck
### UDPDomain=$(host ${TRAP_HOST} | awk '{print $3 }' | awk -F . ' {printf "%02x%02x%02x%02x%04x\n", $1, $2, $3, $4, 162 }')

###############################
# check the connectivity first:
###############################
snmpget ${SNMP_HOST} SNMPv2-MIB::sysName.0
snmpget -r 0 -v 2c -c ${TRAP_COMMUNITY} ${TRAP_HOST} SNMPv2-MIB::sysName.0 || echo "WARNING: notification receiver (snmpd) may not ready?"

set -x  # be verbose

# 1. SNMP-NOTIFICATION-MIB::snmpNotifyTable
#
if ${SNMP_GET} ${SNMP_HOST} SNMP-NOTIFICATION-MIB::snmpNotifyRowStatus.\'${TRAP_HOST}\' | egrep "No Such Instance" ; then
   ${SNMP_SET} ${SNMP_HOST} SNMP-NOTIFICATION-MIB::snmpNotifyRowStatus.\'${TRAP_HOST}\' i createAndWait
else
   ${SNMP_SET} ${SNMP_HOST} SNMP-NOTIFICATION-MIB::snmpNotifyRowStatus.\'${TRAP_HOST}\' = notInService || echo ignored
fi
${SNMP_SET} ${SNMP_HOST} \
    SNMP-NOTIFICATION-MIB::snmpNotifyTag.\'${TRAP_HOST}\' = "${TRAP_HOST}" \
    SNMP-NOTIFICATION-MIB::snmpNotifyType.\'${TRAP_HOST}\' = trap
${SNMP_SET} ${SNMP_HOST} \
    SNMP-NOTIFICATION-MIB::snmpNotifyRowStatus.\'${TRAP_HOST}\' = active

################
### exit
################

# 2. snmpTargetAddrTable
#
if ${SNMP_GET} ${SNMP_HOST} SNMP-TARGET-MIB::snmpTargetAddrRowStatus.\'${TRAP_HOST}\' | egrep "No Such Instance" ; then
   ${SNMP_SET} ${SNMP_HOST} SNMP-TARGET-MIB::snmpTargetAddrRowStatus.\'${TRAP_HOST}\' = createAndWait
else
   ${SNMP_SET} ${SNMP_HOST} SNMP-TARGET-MIB::snmpTargetAddrRowStatus.\'${TRAP_HOST}\' = notInService || echo ignored
fi
#XXX   SNMP-TARGET-MIB::snmpTargetAddrTDomain.\'${TRAP_HOST}\' = SNMPv2-SMI::snmpDomains.1 \
#XXX   SNMP-TARGET-MIB::snmpTargetAddrTAddress.\'${TRAP_HOST}\' x ${UDPDomain} \
#XXX   SNMP-TARGET-MIB::snmpTargetAddrTDomain.\'${TRAP_HOST}\' = TRANSPORT-ADDRESS-MIB::transportDomainUdpIpv6 \
#
#TODO    SNMP-TARGET-MIB::snmpTargetAddrTDomain.\'${TRAP_HOST}\' = TRANSPORT-ADDRESS-MIB::transportDomainUdpIpv6 \
#FIXME transportDomainTcpIpv6 used for test
${SNMP_SET} ${SNMP_HOST} \
    SNMP-TARGET-MIB::snmpTargetAddrTDomain.\'${TRAP_HOST}\' = TRANSPORT-ADDRESS-MIB::transportDomainTcpIpv6 \
    SNMP-TARGET-MIB::snmpTargetAddrTAddress.\'${TRAP_HOST}\' x ${DomainUdpIpv6} \
    SNMP-TARGET-MIB::snmpTargetAddrTagList.\'${TRAP_HOST}\' = "${TRAP_HOST}" \
    SNMP-TARGET-MIB::snmpTargetAddrParams.\'${TRAP_HOST}\' = "${TRAP_HOST}"
${SNMP_SET} ${SNMP_HOST} \
    SNMP-TARGET-MIB::snmpTargetAddrRowStatus.\'${TRAP_HOST}\' = active

################
### exit
################

# 3. SNMP-TARGET-MIB::snmpTargetParamsTable
#
if ${SNMP_GET} ${SNMP_HOST} SNMP-TARGET-MIB::snmpTargetParamsRowStatus.\'${TRAP_HOST}\' | egrep "No Such Instance" ; then
   ${SNMP_SET} ${SNMP_HOST} SNMP-TARGET-MIB::snmpTargetParamsRowStatus.\'${TRAP_HOST}\' i createAndWait
else
   ${SNMP_SET} ${SNMP_HOST} SNMP-TARGET-MIB::snmpTargetParamsRowStatus.\'${TRAP_HOST}\' i notInService || echo ignored
fi
# setup MPModel=SNMPv3 and SecurityModel=SNMPv3
${SNMP_SET} ${SNMP_HOST} \
    SNMP-TARGET-MIB::snmpTargetParamsMPModel.\'${TRAP_HOST}\' = 3 \
    SNMP-TARGET-MIB::snmpTargetParamsSecurityModel.\'${TRAP_HOST}\' = 3 \
    SNMP-TARGET-MIB::snmpTargetParamsSecurityName.\'${TRAP_HOST}\' = ${TRAP_COMMUNITY} \
    SNMP-TARGET-MIB::snmpTargetParamsSecurityLevel.\'${TRAP_HOST}\' = noAuthNoPriv
${SNMP_SET} ${SNMP_HOST} \
    SNMP-TARGET-MIB::snmpTargetParamsRowStatus.\'${TRAP_HOST}\' = active

# 4. Show the related tables now
#
### set -x
# snmptable ${SNMP_HOST} SNMP-NOTIFICATION-MIB::snmpNotifyTable
# snmptable ${SNMP_HOST} SNMP-TARGET-MIB::snmpTargetAddrTable
# snmptable ${SNMP_HOST} SNMP-TARGET-MIB::snmpTargetParamsTable

# 5. Set to 1 to force the agent to save it's persistent data immediately.
#
set +x
echo "To save all this setting, do:"
echo "  ${SNMP_SET} ${SNMP_HOST} UCD-SNMP-MIB::versionSavePersistentData.0 i 1 "

set +x
echo "To see all info, do:"
echo "  ${SNMP_WALK} ${SNMP_HOST} .1.3.6.1.6.3"

${SNMP_GET} ${SNMP_HOST} SNMPv2-MIB::snmpInBadCommunityNames.0 \
    SNMPv2-MIB::snmpOutTraps.0 SNMPv2-MIB::snmpEnableAuthenTraps.0
# SNMPv2-MIB::snmpInBadCommunityNames.0 = Counter32: 36
# SNMPv2-MIB::snmpOutTraps.0 = Counter32: 0
# SNMPv2-MIB::snmpEnableAuthenTraps.0 = INTEGER: enabled(1)
echo

MIBTABLES="snmpTargetAddrTable snmpTargetParamsTable snmpNotifyTable snmpNotifyFilterProfileTable snmpNotifyFilterTable"
for Table in $MIBTABLES; do
    snmptable -C biw 120 ${SNMP_HOST} $Table;
    printf '\n--------------------------\n\n'; done 2>&1 |
    perl -n -e 'if (/(^SNMP table.*$)/) {print "$1::\n";} else {if (length() > 1) {print "\t$_";} else {print;} }'

#####
exit
#####

MIBTABLES="nlmConfigLogTable nlmStatsLogTable nlmLogTable nlmLogVariableTable"
for Table in $MIBTABLES; do
    snmptable -n "snmptrapd" -C biw 120 ${SNMP_HOST} $Table;
    printf '\n--------------------------\n\n'; done 2>&1 |
    perl -n -e 'if (/(^SNMP table.*$)/) {print "$1\"snmptrapd\"::\n";} else {if (length() > 1) {print "\t$_";} else {print;} }'

snmpwalk -n "snmptrapd" ${SNMP_HOST} nlmconf
snmpwalk -n "snmptrapd" ${SNMP_HOST} nlmstat

# vim:tabstop=4 shiftwidth=4 expandtab

# + snmpget -r0 localhost: 'SNMP-NOTIFICATION-MIB::snmpNotifyRowStatus.'\''wd002072'\'''
# + egrep 'No Such Instance'
# + snmpset -r0 localhost: 'SNMP-NOTIFICATION-MIB::snmpNotifyRowStatus.'\''wd002072'\''' = notInService
# SNMP-NOTIFICATION-MIB::snmpNotifyRowStatus.\'wd002072\' = INTEGER: notInService(2)
# + snmpset -r0 localhost: 'SNMP-NOTIFICATION-MIB::snmpNotifyTag.'\''wd002072'\''' = wd002072 'SNMP-NOTIFICATION-MIB::snmpNotifyType.'\''wd002072'\''' = trap
# SNMP-NOTIFICATION-MIB::snmpNotifyTag.\'wd002072\' = STRING: wd002072
# SNMP-NOTIFICATION-MIB::snmpNotifyType.\'wd002072\' = INTEGER: trap(1)
# + snmpset -r0 localhost: 'SNMP-NOTIFICATION-MIB::snmpNotifyRowStatus.'\''wd002072'\''' = active
# SNMP-NOTIFICATION-MIB::snmpNotifyRowStatus.\'wd002072\' = INTEGER: active(1)
# + snmpget -r0 localhost: 'SNMP-TARGET-MIB::snmpTargetAddrRowStatus.'\''wd002072'\'''
# + egrep 'No Such Instance'
# + snmpset -r0 localhost: 'SNMP-TARGET-MIB::snmpTargetAddrRowStatus.'\''wd002072'\''' = notInService
# SNMP-TARGET-MIB::snmpTargetAddrRowStatus.\'wd002072\' = INTEGER: notInService(2)
# + snmpset -r0 localhost: 'SNMP-TARGET-MIB::snmpTargetAddrTDomain.'\''wd002072'\''' = SNMPv2-SMI::snmpDomains.1 'SNMP-TARGET-MIB::snmpTargetAddrTAddress.'\''wd002072'\''' x ac17211900a2 'SNMP-TARGET-MIB::snmpTargetAddrTagList.'\''wd002072'\''' = wd002072 'SNMP-TARGET-MIB::snmpTargetAddrParams.'\''wd002072'\''' = wd002072
# SNMP-TARGET-MIB::snmpTargetAddrTDomain.\'wd002072\' = OID: SNMPv2-TM::snmpUDPDomain
# SNMP-TARGET-MIB::snmpTargetAddrTAddress.\'wd002072\' = Hex-STRING: AC 17 21 19 00 A2
# SNMP-TARGET-MIB::snmpTargetAddrTagList.\'wd002072\' = STRING: wd002072
# SNMP-TARGET-MIB::snmpTargetAddrParams.\'wd002072\' = STRING: wd002072
# + snmpset -r0 localhost: 'SNMP-TARGET-MIB::snmpTargetAddrRowStatus.'\''wd002072'\''' = active
# SNMP-TARGET-MIB::snmpTargetAddrRowStatus.\'wd002072\' = INTEGER: active(1)
# + snmpget -r0 localhost: 'SNMP-TARGET-MIB::snmpTargetParamsRowStatus.'\''wd002072'\'''
# + egrep 'No Such Instance'
# + snmpset -r0 localhost: 'SNMP-TARGET-MIB::snmpTargetParamsRowStatus.'\''wd002072'\''' i notInService
# SNMP-TARGET-MIB::snmpTargetParamsRowStatus.\'wd002072\' = INTEGER: notInService(2)
# + snmpset -r0 localhost: 'SNMP-TARGET-MIB::snmpTargetParamsMPModel.'\''wd002072'\''' = 1 'SNMP-TARGET-MIB::snmpTargetParamsSecurityModel.'\''wd002072'\''' = 2 'SNMP-TARGET-MIB::snmpTargetParamsSecurityName.'\''wd002072'\''' = public 'SNMP-TARGET-MIB::snmpTargetParamsSecurityLevel.'\''wd002072'\''' = noAuthNoPriv
# SNMP-TARGET-MIB::snmpTargetParamsMPModel.\'wd002072\' = INTEGER: 1
# SNMP-TARGET-MIB::snmpTargetParamsSecurityModel.\'wd002072\' = INTEGER: 2
# SNMP-TARGET-MIB::snmpTargetParamsSecurityName.\'wd002072\' = STRING: public
# SNMP-TARGET-MIB::snmpTargetParamsSecurityLevel.\'wd002072\' = INTEGER: noAuthNoPriv(1)
# + snmpset -r0 localhost: 'SNMP-TARGET-MIB::snmpTargetParamsRowStatus.'\''wd002072'\''' = active
# SNMP-TARGET-MIB::snmpTargetParamsRowStatus.\'wd002072\' = INTEGER: active(1)
# 
# To save all this setting, do:
#   snmpset -r0 localhost: UCD-SNMP-MIB::versionSavePersistentData.0 i 1
# To see all info, do:
#   snmpwalk -r0 localhost: .1.3.6.1.6.3
# SNMPv2-MIB::snmpInBadCommunityNames.0 = Counter32: 3
# SNMPv2-MIB::snmpOutTraps.0 = Counter32: 6
# SNMPv2-MIB::snmpEnableAuthenTraps.0 = INTEGER: enabled(1)
# 
# SNMP table: SNMP-TARGET-MIB::snmpTargetAddrTable::
# 
#                 index                  TDomain             TAddress Timeout RetryCount   TagList    Params StorageType RowStatus
#         \'internal0\' SNMPv2-TM::snmpUDPDomain "7F 00 00 01 00 A2 "    1000          5 internal0 internal0    readOnly    active
#         \'internal1\' SNMPv2-TM::snmpUDPDomain "7F 00 00 01 00 A2 "    1000          5 internal1 internal1    readOnly    active
#          \'wd002072\' SNMPv2-TM::snmpUDPDomain "AC 17 21 19 00 A2 "    1500          3  wd002072  wd002072 nonVolatile    active
# 
#         --------------------------
# 
# SNMP table: SNMP-TARGET-MIB::snmpTargetParamsTable::
# 
#                 index MPModel SecurityModel SecurityName SecurityLevel StorageType RowStatus
#         \'internal0\'       1             2       public  noAuthNoPriv    readOnly    active
#         \'internal1\'       1             2       public  noAuthNoPriv    readOnly    active
#          \'wd002072\'       1             2       public  noAuthNoPriv nonVolatile    active
# 
#         --------------------------
# 
# SNMP table: SNMP-NOTIFICATION-MIB::snmpNotifyTable::
# 
#                 index       Tag Type StorageType RowStatus
#         \'internal0\' internal0 trap    readOnly    active
#         \'internal1\' internal1 trap    readOnly    active
#          \'wd002072\'  wd002072 trap nonVolatile    active
# 
#         --------------------------
# 
# SNMP table: SNMP-NOTIFICATION-MIB::snmpNotifyFilterProfileTable::
# 
#                 index  Name    StorType RowStatus
#         \'internal0\' svfua nonVolatile    active
# 
#         --------------------------
# 
# SNMP table: SNMP-NOTIFICATION-MIB::snmpNotifyFilterTable::
# 
#                                                index   Mask     Type StorageType RowStatus
#         \"svfua\".1.3.6.1.4.1.59999.33.1.3.1.2.1.2.2 "fffb" excluded nonVolatile    active
#             \"svfua\".1.3.6.1.4.1.59999.33.1.3.2.0.2     "" included nonVolatile    active
#                  \"link-status\".1.3.6.1.6.3.1.1.5.3     "" included nonVolatile    active
#                  \"link-status\".1.3.6.1.6.3.1.1.5.4     "" included nonVolatile    active
# 
#         --------------------------
# 
