:
# Rmon History testing script
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

echo interface ifIndex.$TSTIF will be tested

echo " "
echo 1. create control entry
snmpset $COMPAR historyControlBucketsRequested.4 i 4 historyControlInterval.4 i 3 \
historyControlDataSource.4 o interfaces.ifTable.ifEntry.ifIndex.$TSTIF \
historyControlStatus.4 i 2  # createRequest
snmptable -Cibw 80  $COMPAR historyControlTable


echo " "
echo 2. validate it
snmpset $COMPAR historyControlStatus.4 i 1  # valid
snmptable -Cibw 80  $COMPAR historyControlTable
echo "Sleep 3, take it chance to get something"
sleep 3
snmpwalk $COMPAR historyControlBucketsGranted
snmptable -Cibw 80  $COMPAR etherHistoryTable
echo "Sleep 6, take it chance to advance"
sleep 6
snmpwalk $COMPAR historyControlBucketsGranted
snmptable -Cibw 80  $COMPAR etherHistoryTable
snmpwalk $COMPAR historyControl


echo " "
echo "Sleep 3, take it chance to get something more"
sleep 3
snmpwalk $COMPAR historyControlBucketsGranted
snmptable -Cibw 80  $COMPAR etherHistoryTable
#
echo 3.b change requested number of buckets
snmpset $COMPAR historyControlBucketsRequested.4 i 2
snmpget $COMPAR historyControlBucketsGranted.4
echo "Sleep 9, take it chance to get something"
sleep 9
# snmpwalk $COMPAR history
snmpwalk $COMPAR historyControlBucketsGranted
snmptable -Cibw 80  $COMPAR etherHistoryTable


echo " "
echo 4. invalidate it
snmpset $COMPAR historyControlStatus.4 i 4
snmpwalk $COMPAR history


echo " "
echo 5. create and validate 2 control entries # (15 buckets every min)
snmpset $COMPAR historyControlBucketsRequested.4 i 15 historyControlInterval.4 i 1 \
historyControlDataSource.4 o interfaces.ifTable.ifEntry.ifIndex.$TSTIF \
historyControlStatus.4 i 1 
# (96 buckets every 15 min)
snmpset $COMPAR historyControlBucketsRequested.2 i 96 historyControlInterval.2 i 15 \
historyControlDataSource.2 o interfaces.ifTable.ifEntry.ifIndex.$TSTIF \
historyControlStatus.2 i 1
snmptable -Cibw 80 $COMPAR historyControlTable
echo "Sleep 16, take them chance to get something"
sleep 16
snmptable -Cibw 80  $COMPAR etherHistoryTable


echo " "
echo 6. create entry and let it to be aged
snmpset $COMPAR historyControlStatus.3 i 2  # createRequest
snmptable -Cibw 80 $COMPAR historyControlTable
echo "Sleep 61, take it chance to be aged"
sleep 61
snmptable -Cibw 80 $COMPAR historyControlTable
snmptable -Cibw 80  $COMPAR etherHistoryTable


echo " "
echo 7. clean everything
snmpset $COMPAR historyControlStatus.2 i 4
snmpset $COMPAR historyControlStatus.3 i 4
snmpset $COMPAR historyControlStatus.4 i 4
snmpwalk $COMPAR history


echo " "
echo "Goodbye, I'm a gonner"
echo " "

