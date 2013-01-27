#!/bin/sh
set -e

./configure --help > configure--help.txt
# ***************************************************************************
# *
# * Cygwin - Building
# *
# ***************************************************************************
# 
# An alternate way to build Net-SNMP for win32 is to use Cygnus's cygwin32
# environment.  Information on the Cygnus cygwin32 environment is available
# on the web at: http://sources.redhat.com/cygwin/.
# 
# Cygwin allows you to compile almost the complete agent and applications.
# The following configure options creates a working set of programs:

set -x
./configure --cache-file=config.cache \
  --with-mib-modules="snmpv3mibs notification notification-log-mib target agent_mibs agentx" \
  --with-out-mib-modules="host host/hr_network host/hr_swrun ucd-snmp/diskio" \
  --with-out-mib-modules="disman/event disman/schedule ucd_snmp" \
  --with-defaults \
  --without-rpm \
  --with-libs="-lws2_32" \
  --disable-embedded-perl --without-perl-modules \
  --without-python-modules --disable-deprecated \
  --disable-snmpv1 \
  --enable-ipv6 --with-transports="TCPIPv6 UDPIPv6" \
  --enable-developer \
  --enable-mini-agent

cp -p configure-summary configure-summary.cygwin
make

ctags -R .
## srcdir=. doxygen doxygen.conf
## ./net-snmp-config --version --mibs

make test

#################
exit
#################

#FIXME#	--with-transports="UDP UDPIPv6 " \
#XXX#   --with-out-mib-modules="host host/hr_network disman/event-mib disman/schedule " \
# If you want to use SNMPv3 auth and privacy features, add:
	# --with-openssl="/usr" \

# If you want to use IPv6 transports, add:
	# --enable-ipv6 --with-transports="TCPIPv6 UDPIPv6"

# Note:  The source code should *not* be in a folder that contains a space.  For
       # example, compiling in your 'My Documents' or your Desktop (usually
       # c:\Documents and Settings\xxxx\Desktop) is not supported.

# --enable-mfd-rewrites \
# --enable-new-features   # Compile in new MIB modules and other experimental

# This has been tested for Windows 98 and Windows NT 4.0.  In order for
# the process part of the host resources MIB to work under NT you will need
# to get hold of the PSAPI.DLL. This available under the download section
# of www.microsoft.com.  The DLL is included with Windows 2000 and XP,
# and is also part of the VC++ distribution.  The IPHLPAPI library is
# part of the "Microsoft Platform SDK", which is also available from
# www.microsoft.com.  See the section "Installing Platform SDK" for details.
# 
# Earlier releases of Cygwin may need to use the configure flag
	# --with-libs="-lregex -libphlpapi"
# but this regular expression support has since been incorporated
# into the main Cygwin package, and it is no longer necessary to
# include it separately.
# 
# If the folder that Net-SNMP was installed to is ever changed, modify the 
# system environment variables or registry keys as explained in the 
# 'Configuration_Overview.html' file located in win32/dist/htmlhelp.

# ./configure-summary

  SNMP Versions Supported:    2c 3
  Net-SNMP Version:           5.4.3
  Building for:               cygwin
  Network transport support:  Callback TCP UDP TCPIPv6 UDPIPv6
  SNMPv3 Security Modules:     usm
  Agent MIB code:             snmpv3mibs notification notification-log-mib target agent_mibs agentx default_modules =>  snmpv3/snmpEngine snmpv3/snmpMPDStats snmpv3/usmStats snmpv3/usmConf snmpv3/usmUser notification/snmpNotifyTable snmp-notification-mib/snmpNotifyFilterTable notification/snmpNotifyFilterProfileTable notification-log-mib/notification_log notification/snmpNotifyTable target/snmpTargetAddrEntry target/snmpTargetParamsEntry target/target target/target_counters agent/nsTransactionTable agent/nsModuleTable agent/nsDebug agent/nsCache agent/nsLogging agent/nsVacmAccessTable agentx/master agentx/subagent snmpv3mibs mibII/snmp_mib mibII/system_mib mibII/sysORTable mibII/vacm_vars mibII/vacm_conf utilities/execute
  Embedded Perl support:      disabled
  SNMP Perl modules:          disabled
  SNMP Python modules:        disabled
  Authentication support:     MD5
  Encryption support:         
