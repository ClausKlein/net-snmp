#!/bin/sh
#XXX#
set -e  # exit on error

##################################################################################
# usage: start the agentX master agent and than the subagent2 first! ck
# if you wand net-snmp v5.6.x as master agent do:
#   sudo agent/snmpd -f -Le -C '--authuser=read,write unsec noauth' --createUser=unsec --master=agentx -Dagentx localhost:4700
## SNMP_ARGS='-v3 -r0 -t 5 -u unsec localhost:4700'
##################################################################################

SNMP_ARGS='-v3 -r0 -t 5 -n subagent localhost'
SNMP_ROW_INDEX=${1:-mylocalhost}
ETC_HOSTS_FILE=${2:-/tmp/hosts}

set -u  # undefined vars are error

cp /etc/hosts ${ETC_HOSTS_FILE} 

printf "\n#Setup\n"
#XXX#
set -x  # be verbose

if [ ${SNMP_ROW_INDEX} == "testhost" ]; then
  snmpset ${SNMP_ARGS} \
    NET-SNMP-EXAMPLES-MIB::netSnmpHostRowStatus.\"${SNMP_ROW_INDEX}\" = destroy
fi

printf "\n#Pre-condition: row must not exist!\n"
snmpget ${SNMP_ARGS} \
    NET-SNMP-EXAMPLES-MIB::netSnmpHostRowStatus.\"${SNMP_ROW_INDEX}\" |
        grep -q 'No Such' || snmpset ${SNMP_ARGS} \
            NET-SNMP-EXAMPLES-MIB::netSnmpHostRowStatus.\"${SNMP_ROW_INDEX}\" = destroy

printf "\n#Test case: create non-existing table with RowStatus as single shot\n"
snmpset ${SNMP_ARGS} \
    NET-SNMP-EXAMPLES-MIB::netSnmpHostRowStatus.\"${SNMP_ROW_INDEX}\" = createAndGo \
    NET-SNMP-EXAMPLES-MIB::netSnmpHostAddressType.\"${SNMP_ROW_INDEX}\" = ipv6 \
    NET-SNMP-EXAMPLES-MIB::netSnmpHostAddress.\"${SNMP_ROW_INDEX}\" x 0000000000000000000000000000000100000001  ### x 7f000001

printf "\n#Post-condition: row must exist and in state active!\n"
snmpget ${SNMP_ARGS} \
    NET-SNMP-EXAMPLES-MIB::netSnmpHostRowStatus.\"${SNMP_ROW_INDEX}\" | grep -qw 'active'

grep -w ${SNMP_ROW_INDEX} ${ETC_HOSTS_FILE}
COUNT=$(grep -cw ${SNMP_ROW_INDEX} ${ETC_HOSTS_FILE})
test ${COUNT} -eq 0 && printf "\n#ERROR: ${SNMP_ROW_INDEX} not found!\n\n"

snmptable -C biw 100 ${SNMP_ARGS} NET-SNMP-EXAMPLES-MIB::netSnmpHostsTable

printf "\n#Cleanup\n"
snmpset ${SNMP_ARGS} \
    NET-SNMP-EXAMPLES-MIB::netSnmpHostRowStatus.\"${SNMP_ROW_INDEX}\" = destroy

grep -w ${SNMP_ROW_INDEX} ${ETC_HOSTS_FILE} || printf "\n#OK\n"

cat ${ETC_HOSTS_FILE}

snmptable -C biw 100 ${SNMP_ARGS} NET-SNMP-EXAMPLES-MIB::netSnmpHostsTable

####

snmpset ${SNMP_ARGS} \
    NET-SNMP-EXAMPLES-MIB::netSnmpHostRowStatus.\"${SNMP_ROW_INDEX}\" = createAndWait

snmpset ${SNMP_ARGS} \
    NET-SNMP-EXAMPLES-MIB::netSnmpHostAddress.\"${SNMP_ROW_INDEX}\" x 7f000001 \
    NET-SNMP-EXAMPLES-MIB::netSnmpHostAddressType.\"${SNMP_ROW_INDEX}\" = ipv4

snmpset ${SNMP_ARGS} \
    NET-SNMP-EXAMPLES-MIB::netSnmpHostRowStatus.\"${SNMP_ROW_INDEX}\" = active

####

snmpwalk ${SNMP_ARGS} NET-SNMP-EXAMPLES-MIB::netSnmpExamples

snmpset ${SNMP_ARGS} \
    NET-SNMP-EXAMPLES-MIB::netSnmpExampleString.0 = "Test commit failed while multi set request" \
    NET-SNMP-EXAMPLES-MIB::netSnmpExampleInteger.0 = 0 \
    NET-SNMP-EXAMPLES-MIB::netSnmpExampleSleeper.0 = 1 \
        || echo Expected

####
exit
####

