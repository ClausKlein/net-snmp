/* module to include the modules relavent to the mib-II mib(s) */
config_require(system)
config_require(sysORTable)
config_require(at)
config_require(interfaces)
config_require(snmp_mib)
config_require(tcp)
config_require(icmp)
config_require(ip)
config_require(udp)
config_require(vacm_vars)

void init_mibII  __P((void));
  
