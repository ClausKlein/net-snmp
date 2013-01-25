#!/bin/sh
set -x  # be verbose
set -u  # undefined is an error
set -e  # exit on error!

###XXX### SRCDIR=../net-snmp
SRCDIR=$(realpath ..)
### SRCDIR=.

DIRNAME=${PWD##*/}
UNAME=$(uname)

if [ -x ${SRCDIR}/configure ]; then
  ###FIXME export CPPFLAGS="-Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wno-sign-compare -Wno-pointer-sign -Wno-cast-qual"
  export MIBDIRS=${SRCDIR}/mibs
  ${SRCDIR}/configure --help > configure--help-${DIRNAME}-${UNAME}.log

  ${SRCDIR}/configure --cache-file="config.cache" \
    --prefix=/opt/local \
    --with-cflags="-I/opt/local/include -g -Wall -Wextra" \
    --with-ldflags="-L/opt/local/lib" \
    --with-defaults \
    --disable-snmpv1 \
    --without-rpm \
    --without-perl-modules \
    --without-python-modules \
    --with-out-mib-modules="ucd-snmp/diskio" \
    --with-out-mib-modules="host" \
    --without-kmem-usage \
    --with-out-mib-modules="mibII/icmp host/hr_swrun" \
    --with-out-mib-modules="disman/event disman/schedule" \
    --with-mib-modules="disman" \
    --with-mib-modules="Rmon" \
    --with-out-mib-modules="mibII" \
    --with-out-mib-modules="ucd-snmp" \
    --with-out-mib-modules="ucd-snmp/proxy" \
    --with-out-mib-modules="ucd-snmp/versioninfo" \
    --with-out-mib-modules="examples/netSnmpHostsTable" \
    --enable-ipv6 \
    --with-transports="TCPIPv6 UDPIPv6" \
    --with-security-modules="usm" \
    --with-mib-modules="Rmon disman agent_mibs agentx notification notification-log-mib target" \
    --enable-developer \
    --enable-mib-config-checking \
    --enable-mini-agent             ### Build a minimal agent.

####FIXME   --enable-reentrant \

###XXX### not yet! ck
#   --with-transports="TCPIPv6 UDPIPv6 SSH TLSTCP DTLSUDP" \
#   --with-security-modules="usm tsm" \

#TBD#
#   --enable-mib-config-debug \
#   --with-out-mib-modules="host/hr_proc ucd-snmp/diskio" \
#   --with-mib-modules="host/hr_network host/hr_device" \

#   --with-mib-modules="ucd-snmp/lmSensors" \
# checking for sensors support... checking for sensors/sensors.h... (cached) no
# configure: error: asked to use lm_sensors but I couldn't find sensors/sensors.h

###XXX Build a minimal agent.  --enable-mini-agent
# claus-kleins-macbook-pro:build clausklein$ ./net-snmp-config --version --mibs
# 5.4.4
# :SNMP-TARGET-MIB:SNMPv2-MIB:IF-MIB:IP-MIB:TCP-MIB:UDP-MIB:SNMP-NOTIFICATION-MIB:SNMPv2-TM:SNMP-VIEW-BASED-ACM-MIB:SNMP-COMMUNITY-MIB:SNMP-FRAMEWORK-MIB:SNMP-MPD-MIB:SNMP-USER-BASED-SM-MIB

  # Compile in the given SNMP transport
  # Generate IPv6 ready version.
  #Note: --with-defaults         Use defaults for prompted values.

  ###FIXME configure: error: The SSH transport requires the libssh2 library to be available
  # --enable-developer 	Turns on super-duper-extra-compile-warnings

  # --with-out-mib-modules="list"   Compile without these mib modules.
  #with: --enable-mini-agent --without-mib-modules="agentx notification notification-log-mib target" \
  ##### :SNMPv2-MIB:RFC1213-MIB:SNMP-VIEW-BASED-ACM-MIB:SNMP-COMMUNITY-MIB:SNMP-MPD-MIB:SNMP-USER-BASED-SM-MIB:SNMP-FRAMEWORK-MIB

  ###XXX Build a minimal agent.  --enable-mini-agent
  ### :SNMP-TARGET-MIB:SNMPv2-MIB:RFC1213-MIB:SNMP-NOTIFICATION-MIB:SNMPv2-TM:SNMP-VIEW-BASED-ACM-MIB:SNMP-COMMUNITY-MIB:SNMP-MPD-MIB:SNMP-USER-BASED-SM-MIB:SNMP-FRAMEWORK-MIB

  ###TBD --enable-mfd-rewrites \
  ###TBD --enable-new-features   # Compile in new MIB modules and other experimental

  cp -p configure-summary configure-summary-${DIRNAME}-${UNAME}.log
  cp -p config.log config-${DIRNAME}-${UNAME}.log
  ./net-snmp-config --version --mibs > net-snmp-config-mibs-${DIRNAME}-${UNAME}.log

  make
  ### ctags -R .
  ## srcdir=. doxygen doxygen.conf


  ### ./net-snmp-config --version --mibs
  ### 5.7
  ### :SNMP-TARGET-MIB:SNMPv2-MIB:IF-MIB:IP-MIB:TCP-MIB:UDP-MIB:HOST-RESOURCES-MIB:NOTIFICATION-LOG-MIB:DISMAN-EVENT-MIB:DISMAN-SCHEDULE-MIB:SNMP-NOTIFICATION-MIB:SNMPv2-TM:UCD-SNMP-MIB:UCD-DEMO-MIB:NET-SNMP-AGENT-MIB:SNMP-MPD-MIB:SNMP-USER-BASED-SM-MIB:SNMP-FRAMEWORK-MIB:SNMP-VIEW-BASED-ACM-MIB:SNMP-COMMUNITY-MIB:NET-SNMP-PASS-MIB:NET-SNMP-EXTEND-MIB:UCD-DLMOD-MIB:NET-SNMP-VACM-MIB

  #################
  ###XXX make test
  #################
fi


#NOTE: see ./README.osX and
##### configure--help-net-snmp-Darwin.log
##### or build/configure--help-build-Darwin.log
##### config-net-snmp-Darwin.log
##### configure-summary-net-snmp-Darwin.log
##### or build/configure-summary-build-Darwin.log

#NOTE: cat ./configure-summary

# SNMP Versions Supported:    2c 3
# Building for:               darwin9
# Net-SNMP Version:           5.8.dev
# Network transport support:  Callback Unix Alias TCP UDP TCPIPv6 UDPIPv6
#   IPv4Base SocketBase TCPBase UDPIPv4Base UDPBase IPv6Base
# SNMPv3 Security Modules:     usm
# Agent Module list: mk/module_list_code.mk
#
# Agent MIB code:             Rmon disman agent_mibs agentx notification notification-log-mib target default_modules =>
#    Rmon/rows Rmon/agutil Rmon/statistics Rmon/alarmTable Rmon/history
#    Rmon/event disman/event-mib disman/expression-mib disman/schedule
#    agent/nsTransactionTable agent/nsModuleTable agent/nsDebug agent/nsCache
#    agent/nsLogging agent/nsVacmAccessTable agentx/master agentx/subagent
#    notification/snmpNotifyTable snmp-notification-mib/snmpNotifyFilterTable
#    notification/snmpNotifyFilterProfileTable
#    notification-log-mib/notification_log notification/snmpNotifyTable
#    target/target_counters_5_5 target/snmpTargetAddrEntry
#    target/snmpTargetParamsEntry target/target snmpv3mibs mibII/snmp_mib
#    mibII/system_mib mibII/sysORTable mibII/vacm_vars mibII/vacm_conf
#
#  MYSQL Trap Logging:         unavailable
#  Embedded Perl support:      disabled
#  SNMP Perl modules:          disabled
#  SNMP Python modules:        disabled
#  Crypto support from:        crypto
#  Authentication support:     MD5 SHA1
#  Encryption support:         DES AES
#  Local DNSSEC validation:    disabled

# createUser [-e ENGINEID] username (MD5|SHA) authpassphrase [DES|AES] [privpassphrase]
#
sudo killall snmpd || echo ignored
sudo ./agent/snmpd -Lf ./snmpd.log -p ${PWD}/snmpd.pid -C -Dagentx,read_config,ucd-snmp,register_mib,snmp_,sess_,host,access:etherStatsTable  \
  '--view all included .1 80' \
  '--rwuser clausklein noAuthNoPriv -V all "*"' \
  '--createUser=clausklein SHA "---none---" AES "---none---"' \
  --persistentDir="${PWD}" \
  -c ./snmpd.conf -C --defSecurityModel=usm \
  'tcp6:[::1]:161'

sleep 3

./apps/snmpstatus -m ALL -M ../mibs -r0 -v3 -u clausklein -l noAuthNoPriv --defSecurityModel=usm -Dssh 'tcp6:[::1]:161' || echo ignored
# [TCP/IPv6: [::1]:161]=>[Darwin claus-kleins-macbook-pro.local 9.8.0 Darwin Kernel Version 9.8.0: Wed Jul 15 16:55:01 PDT 2009; root:xnu-1228.15.4~1/RELEASE_I386 i386] Up: 0:02:42.29
# Interfaces: 7, Recv/Trans packets: 36495/34026 | IP: 32241/29285
# 4 interfaces are down!
# claus-kleins-macbook-pro:net-snmp clausklein$

MIBTABLES="atTable egpNeighTable icmpMsgStatsTable icmpStatsTable ifRcvAddressTable ifStackTable
ifTable ifTestTable ifXTable inetCidrRouteTable ipAddrTable ipAddressPrefixTable ipAddressTable
ipCidrRouteTable ipDefaultRouterTable ipForwardTable ipIfStatsTable ipNetToMediaTable
ipNetToPhysicalTable ipRouteTable ipSystemStatsTable ipv4InterfaceTable ipv6InterfaceTable
ipv6RouterAdvertTable ipv6ScopeZoneIndexTable sysORTable tcpConnTable tcpConnectionTable
tcpListenerTable udpTable
"

for m in $MIBTABLES ; do \
    echo "./apps/snmptable -Cibw 123 -m ALL -M ../mibs -v3 -u clausklein -l noAuthNoPriv --defSecurityModel=usm 'tcp6:[::1]:161' $m"; \
    ./apps/snmptable -Cibw 123 -m ALL -M ../mibs -v3 -u clausklein -l noAuthNoPriv --defSecurityModel=usm 'tcp6:[::1]:161' $m; echo; \
done > MIBII-Tables-${DIRNAME}-${UNAME}.log

# see MIBII-Tables-net-snmp-Darwin.log
#  or build/MIBII-Tables-build-Darwin.log


