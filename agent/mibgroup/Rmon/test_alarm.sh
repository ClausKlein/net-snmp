:

# Rmon Alarms/Events testing script
# $Log$
# Revision 5.0  2002/04/20 07:30:01  hardaker
# cvs file version number change
#
# Revision 1.1  2001/05/09 19:36:13  slif
# Include Alex Rozin's Rmon.
#
#

#Only parameter: number of interface (ifIndex) to be tested.
#Default: 1

TSTIF=1
COMPAR="-m +RMON2-MIB -v3 localhost "

if [ "X"${1} = "X" ] ; then
    echo got default parameter : $TSTIF
else
    TSTIF=$1
fi

#testing parameters:
ETHIND=1
EVNIND=7
ALRIND=2
LOWLIMIT=4800
HILIMIT=4900
INTERVAL=3
WAITTIME=17

echo interface ifIndex.$TSTIF will be tested

###FIXME bus error at write_etherStatsEntry()
snmpset $COMPAR etherStatsStatus.1 i 4 \
            etherStatsDataSource.2 o interfaces.ifTable.ifEntry.ifIndex.2

for s in $(seq 4 -1 1); do
    snmpset $COMPAR etherStatsStatus.$s i $s \
        etherStatsDataSource.5 o interfaces.ifTable.ifEntry.ifIndex.2
done

echo " "
echo 1. Create etherStatsEntry  # createRequest(2)
snmpset $COMPAR \
	etherStatsStatus.$ETHIND i 2 \
	etherStatsDataSource.$ETHIND o interfaces.ifTable.ifEntry.ifIndex.$TSTIF
snmpset $COMPAR etherStatsStatus.$ETHIND i 1    # valid(1)
snmpwalk $COMPAR statistics
snmptable -Cibw 80 $COMPAR etherStatsTable
snmptable -Cibw 80 $COMPAR etherStats2Table

echo 2. Create event control entry: # valid(1) logandtrap(4)
snmpset $COMPAR \
	eventStatus.$EVNIND i 1 \
	eventDescription.$EVNIND s "Alarms" \
	eventType.$EVNIND i 4 \
	eventOwner.$EVNIND s "Alex"
snmptable -Cibw 80 $COMPAR eventTable

echo 3. Create alarm entry: # valid(1) deltaValue(2)
snmpset $COMPAR \
	alarmStatus.$ALRIND i 1 \
	alarmInterval.$ALRIND i $INTERVAL \
	alarmVariable.$ALRIND o rmon.statistics.etherStatsTable.etherStatsEntry.etherStatsPkts.$ETHIND \
	alarmSampleType.$ALRIND i 2 \
	alarmFallingThreshold.$ALRIND i $LOWLIMIT \
	alarmRisingThreshold.$ALRIND i $HILIMIT \
	alarmRisingEventIndex.$ALRIND i $EVNIND \
	alarmFallingEventIndex.$ALRIND i $EVNIND
snmptable -Cibw 80 $COMPAR alarmTable

echo 4. Sleep $WAITTIME to collect log.
echo    This $WAITTIME seconds you may enjoy with Rmon traps.
sleep $WAITTIME

echo 5. Check log:
# snmpwalk $COMPAR logTable
snmptable -Cibw 80 $COMPAR logTable
snmptable -Cibw 80 $COMPAR eventTable

echo 6.  clean everything: # set to invalid(4)
snmpset $COMPAR alarmStatus.$ALRIND i 4
snmpset $COMPAR eventStatus.$EVNIND i 4
snmpset $COMPAR etherStatsStatus.$ETHIND i 4
snmpset $COMPAR etherStatsStatus.2 i 4
snmpwalk $COMPAR rmon
	
echo " "
echo "Goodbye, I'm a gonner"
echo " "

