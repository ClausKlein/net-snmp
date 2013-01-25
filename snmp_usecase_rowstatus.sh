#!/bin/sh
set -u
set -e

## SNMP_ARGS='-v3 -r0 -u unsec localhost:4700'

SNMP_ARGS='-v3 -r0 localhost'

printf "\n#Setup\n"
snmpset ${SNMP_ARGS} \
    SNMP-NOTIFICATION-MIB::snmpNotifyFilterProfileRowStatus.\'internal0\' = destroy

printf "\n#Pre-condition: row must not exist!\n"
snmpget ${SNMP_ARGS} \
    SNMP-NOTIFICATION-MIB::snmpNotifyFilterProfileRowStatus.\'internal0\' |
        grep 'No Such'

printf "\n#Test case: create non-existing table with RowStatus as single shot\n"
snmpset ${SNMP_ARGS} \
    SNMP-NOTIFICATION-MIB::snmpNotifyFilterProfileRowStatus.\'internal0\' = createAndGo \
    SNMP-NOTIFICATION-MIB::snmpNotifyFilterProfileName.\'internal0\' =  svfua

printf "\n#Post-condition: row must exist and in state active!\n"
snmptable -C biw 100 ${SNMP_ARGS} SNMP-NOTIFICATION-MIB::snmpNotifyFilterProfileTable

printf "\n#Cleanup\n"
snmpset ${SNMP_ARGS} \
    SNMP-NOTIFICATION-MIB::snmpNotifyFilterProfileRowStatus.\'internal0\' = destroy

