#!/bin/sh
set -x  # be verbose
set -u  # undefined is an error
set -e  # exit on error!

SRCDIR=../
###XXX### SRCDIR=/media/c17e3c7a-8f76-39f8-aa8b-5ff7e121958d/Users/clausklein/Workspace/c/net-snmp
### SRCDIR=.

DIRNAME=${PWD##*/}
UNAME=$(uname)

if [ -x ${SRCDIR}/configure ]; then
  ###FIXME export CPPFLAGS="-Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-but-set-variable -Wno-sign-compare -Wno-pointer-sign -Wno-cast-qual"
  export MIBDIRS=${SRCDIR}/mibs
  ${SRCDIR}/configure --help > configure--help-${DIRNAME}-${UNAME}.log

  ${SRCDIR}/configure --cache-file="config.cache" \
    --prefix=/usr/local \
    --with-cflags="-I/usr/local/include -g -Wall -Wextra" \
    --with-ldflags="-L/usr/local/lib" \
    --with-defaults \
    --disable-snmpv1 \
    --without-rpm \
    --without-perl-modules \
    --without-python-modules \
    --with-out-mib-modules="ucd-snmp/diskio" \
    --with-out-mib-modules="host" \
    --with-mib-modules="disman/nslookup-mib disman/ping-mib disman/traceroute-mib" \
    --with-mib-modules="disman" \
    --with-mib-modules="Rmon" \
    --with-mib-modules="mibII" \
    --with-out-mib-modules="ucd-snmp" \
    --with-out-mib-modules="ucd-snmp/proxy" \
    --with-out-mib-modules="ucd-snmp/versioninfo" \
    --with-out-mib-modules="examples/netSnmpHostsTable" \
    --enable-ipv6 \
    --with-transports="TCPIPv6 UDPIPv6" \
    --with-mib-modules="mibII Rmon disman agent_mibs agentx notification notification-log-mib target" \
    --enable-developer \
    --enable-mib-config-checking \
    --with-libs="-lpthread -lrt"
###XXX###    --enable-mini-agent             ###TBD### Remove all non-essential code features.

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
  ### :SNMP-TARGET-MIB:SNMPv2-MIB:IF-MIB:IP-MIB:TCP-MIB:UDP-MIB:HOST-RESOURCES-MIB:NOTIFICATION-LOG-MIB:DISMAN-EVENT-MIB:DISMAN-SCHEDULE-MIB:SNMP-NOTIFICATION-MIB:SNMPv2-TM:UCD-SNMP-MIB:UCD-DEMO-MIB:NET-SNMP-AGENT-MIB:HOST-RESOURCES-TYPES:SNMP-MPD-MIB:SNMP-USER-BASED-SM-MIB:SNMP-FRAMEWORK-MIB:SNMP-VIEW-BASED-ACM-MIB:SNMP-COMMUNITY-MIB:IPV6-ICMP-MIB:IPV6-MIB:IPV6-TCP-MIB:IPV6-UDP-MIB:IP-FORWARD-MIB:NET-SNMP-PASS-MIB:NET-SNMP-EXTEND-MIB:UCD-DLMOD-MIB:NET-SNMP-VACM-MIB

  #################
  make test
  exit
  #################
fi


#NOTE: see
##### configure--help-net-snmp-Linux.log
##### config-net-snmp-Linux.log
##### configure-summary-net-snmp-Linux.log
##### configure-summary-net-snmp-Linux.log
##### or build/configure-summary-build-Linux.log

#NOTE: cat ./configure-summary

# SNMP Versions Supported:    2c 3
# Building for:               linux
# Net-SNMP Version:           5.7
# Network transport support:  Callback Unix Alias TCP UDP TCPIPv6 UDPIPv6 SSH
#   IPv4Base SocketBase TCPBase UDPIPv4Base UDPBase IPv6Base
# SNMPv3 Security Modules:     usm
# Agent MIB code:             agentx notification notification-log-mib target
#   ucd-snmp/proxy default_modules =>  agentx/master agentx/subagent
#   notification/snmpNotifyTable snmp-notification-mib/snmpNotifyFilterTable
#   notification/snmpNotifyFilterProfileTable
#   notification-log-mib/notification_log notification/snmpNotifyTable
#   target/target_counters_5_5 target/snmpTargetAddrEntry
#   target/snmpTargetParamsEntry target/target snmpv3mibs mibII ucd_snmp
#   notification notification-log-mib target agent_mibs agentx utilities host
# MYSQL Trap Logging:         unavailable
# Embedded Perl support:      disabled
# SNMP Perl modules:          disabled
# SNMP Python modules:        disabled
# Crypto support from:        crypto
# Authentication support:     MD5 SHA1
# Encryption support:         DES AES
# Local DNSSEC validation:    disabled

# Test Summary Report
# -------------------
# SNMPv2c set of system.sysContact.0                              (Wstat: 256 Tests: 4 Failed: 1)
#   Failed test:  3
#   Non-zero exit status: 1
# SNMPv3 snmptrapd USM user management with snmpusm               (Wstat: 256 Tests: 12 Failed: 6)
#   Failed tests:  5-6, 8-11
#   Non-zero exit status: 1
# SNMPv3 traps received and sent by tools                         (Wstat: 256 Tests: 4 Failed: 2)
#   Failed tests:  2-3
#   Non-zero exit status: 1
# snmptrapd traphandle: launching external shell script           (Wstat: 256 Tests: 3 Failed: 2)
#   Failed tests:  1-2
#   Non-zero exit status: 1
# extending agent functionality with extend                       (Wstat: 256 Tests: 6 Failed: 3)
#   Failed tests:  1-3
#   Non-zero exit status: 1
# com2sec directive                                               (Wstat: 256 Tests: 33 Failed: 2)
#   Failed tests:  29, 31
#   Non-zero exit status: 1
# com2sec6 directive                                              (Wstat: 0 Tests: 33 Failed: 2)
#   Failed tests:  29, 31
# com2secunix directive                                           (Wstat: 256 Tests: 12 Failed: 1)
#   Failed test:  11
#   Non-zero exit status: 1
# AgentX trap sending support using agentxtrap                    (Wstat: 256 Tests: 2 Failed: 1)
#   Failed test:  1
#   Non-zero exit status: 1
# Files=66, Tests=224, 89 wallclock secs ( 0.23 usr  0.10 sys +  7.42 cusr  3.82 csys = 11.57 CPU)
# Result: FAIL
#
# We failed these 9 tests:
#   SNMPv2c set of system.sysContact.0 ( /media/c17e3c7a-8f76-39f8-aa8b-5ff7e121958d/Users/clausklein/Workspace/c/net-snmp/testing/fulltests/default/T0141snmpv2cset_simple )
#   SNMPv3 snmptrapd USM user management with snmpusm ( /media/c17e3c7a-8f76-39f8-aa8b-5ff7e121958d/Users/clausklein/Workspace/c/net-snmp/testing/fulltests/default/T035snmpv3trapdusermgmt_simple )
#   SNMPv3 traps received and sent by tools ( /media/c17e3c7a-8f76-39f8-aa8b-5ff7e121958d/Users/clausklein/Workspace/c/net-snmp/testing/fulltests/default/T050snmpv3trap_simple )
#   snmptrapd traphandle: launching external shell script ( /media/c17e3c7a-8f76-39f8-aa8b-5ff7e121958d/Users/clausklein/Workspace/c/net-snmp/testing/fulltests/default/T059trapdtraphandle_simple )
#   extending agent functionality with extend ( /media/c17e3c7a-8f76-39f8-aa8b-5ff7e121958d/Users/clausklein/Workspace/c/net-snmp/testing/fulltests/default/T065agentextend_simple )
#   com2sec directive ( /media/c17e3c7a-8f76-39f8-aa8b-5ff7e121958d/Users/clausklein/Workspace/c/net-snmp/testing/fulltests/default/T070com2sec_simple )
#   com2sec6 directive ( /media/c17e3c7a-8f76-39f8-aa8b-5ff7e121958d/Users/clausklein/Workspace/c/net-snmp/testing/fulltests/default/T071com2sec6_simple )
#   com2secunix directive ( /media/c17e3c7a-8f76-39f8-aa8b-5ff7e121958d/Users/clausklein/Workspace/c/net-snmp/testing/fulltests/default/T072com2secunix_simple )
#   AgentX trap sending support using agentxtrap ( /media/c17e3c7a-8f76-39f8-aa8b-5ff7e121958d/Users/clausklein/Workspace/c/net-snmp/testing/fulltests/default/T114agentxagentxtrap_simple )


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
# [TCP/IPv6: [::1]:161]=>[Linux claus-MacBookPro 2.6.35-30-generic-pae #54-Ubuntu SMP Tue Jun 7 20:28:33 UTC 2011 i686] Up: 0:00:00.57
# Interfaces: 3, Recv/Trans packets: 0/22566 | IP: 22534/22534
# 2 interfaces are down!
# linux-on-macbook-pro:net-snmp clausklein$ 

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

# see MIBII-Tables-net-snmp-Linux.log
#  or build/MIBII-Tables-build-Linux.log


