/*
 * snmptrapd.c - receive and log snmp traps
 *
 */
/*****************************************************************
	Copyright 1989, 1991, 1992 by Carnegie Mellon University

                      All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of CMU not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
CMU BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.
******************************************************************/
#include <config.h>

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#if HAVE_SYS_SOCKIO_H
#include <sys/sockio.h>
#endif
#if HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#include <stdio.h>
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif
#if HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif
#include <sys/param.h>
#if HAVE_SYSLOG_H
#include <syslog.h>
#endif
#if HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif
#include <net/if.h>
#include <netdb.h>
#if HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#if HAVE_FCNTL_H
#include <fcntl.h>
#endif
#include <signal.h>
#include <errno.h>

#include "asn1.h"
#include "snmp_api.h"
#include "snmp_impl.h"
#include "snmp_client.h"
#include "mib.h"
#include "snmp.h"
#include "party.h"
#include "context.h"
#include "acl.h"
#include "system.h"
#include "version.h"
#include "snmptrapd_handlers.h"
#include "read_config.h"
#include "snmp_debug.h"
#include "snmpusm.h"
#include "tools.h"
#include "lcd_time.h"
#include "transform_oids.h"
#include "snmpv3.h"
#include "callback.h"

#ifndef BSD4_3
#define BSD4_2
#endif

#ifndef FD_SET

typedef long	fd_mask;
#define NFDBITS	(sizeof(fd_mask) * NBBY)	/* bits per mask */

#define	FD_SET(n, p)	((p)->fds_bits[(n)/NFDBITS] |= (1 << ((n) % NFDBITS)))
#define	FD_CLR(n, p)	((p)->fds_bits[(n)/NFDBITS] &= ~(1 << ((n) % NFDBITS)))
#define	FD_ISSET(n, p)	((p)->fds_bits[(n)/NFDBITS] & (1 << ((n) % NFDBITS)))
#define FD_ZERO(p)      memset((p), 0, sizeof(*(p)))
#endif

int Print = 0;
int Syslog = 0;
int Event = 0;
int dropauth = 0;
int running = 1;

/*
 * These definitions handle 4.2 systems without additional syslog facilities.
 */
#ifndef LOG_CONS
#define LOG_CONS	0	/* Don't bother if not defined... */
#endif
#ifndef LOG_PID
#define LOG_PID		0	/* Don't bother if not defined... */
#endif
#ifndef LOG_LOCAL0
#define LOG_LOCAL0	0
#endif
#ifndef LOG_LOCAL1
#define LOG_LOCAL1	0
#endif
#ifndef LOG_LOCAL2
#define LOG_LOCAL2	0
#endif
#ifndef LOG_LOCAL3
#define LOG_LOCAL3	0
#endif
#ifndef LOG_LOCAL4
#define LOG_LOCAL4	0
#endif
#ifndef LOG_LOCAL5
#define LOG_LOCAL5	0
#endif
#ifndef LOG_LOCAL6
#define LOG_LOCAL6	0
#endif
#ifndef LOG_LOCAL7
#define LOG_LOCAL7	0
#endif
#ifndef LOG_DAEMON
#define LOG_DAEMON	0
#endif

/* Include an extra Facility variable to allow command line adjustment of
   syslog destination */
int Facility = LOG_LOCAL0;

struct timeval Now;

void init_syslog(void);

void update_config (int a);

const char *
trap_description(int trap)
{
    switch(trap){
	case SNMP_TRAP_COLDSTART:
	    return "Cold Start";
	case SNMP_TRAP_WARMSTART:
	    return "Warm Start";
	case SNMP_TRAP_LINKDOWN:
	    return "Link Down";
	case SNMP_TRAP_LINKUP:
	    return "Link Up";
	case SNMP_TRAP_AUTHFAIL:
	    return "Authentication Failure";
	case SNMP_TRAP_EGPNEIGHBORLOSS:
	    return "EGP Neighbor Loss";
	case SNMP_TRAP_ENTERPRISESPECIFIC:
	    return "Enterprise Specific";
	default:
	    return "Unknown Type";
    }
}


struct snmp_pdu *
snmp_clone_pdu2(struct snmp_pdu *pdu,
		int command)
{
    struct variable_list *var, *newvar;
    struct snmp_pdu *newpdu;

    /* clone the pdu */
    newpdu = (struct snmp_pdu *)malloc(sizeof(struct snmp_pdu));
    if (newpdu == NULL) return NULL;
    memmove(newpdu, pdu, sizeof(struct snmp_pdu));
    newpdu->variables = NULL;
    newpdu->command = command;
    newpdu->reqid = pdu->reqid;
    newpdu->errstat = SNMP_DEFAULT_ERRSTAT;
    newpdu->errindex = SNMP_DEFAULT_ERRINDEX;
    var = pdu->variables;

    newpdu->variables = newvar = (struct variable_list *)malloc(sizeof(struct variable_list));
    memmove(newvar, var, sizeof(struct variable_list));
    if (var->name != NULL){
	newvar->name = (oid *)malloc(var->name_length * sizeof(oid));
	memmove(newvar->name, var->name, var->name_length * sizeof(oid));
    }
    if (var->val.string != NULL){
	newvar->val.string = (u_char *)malloc(var->val_len);
	memmove(newvar->val.string, var->val.string, var->val_len);
    }
    newvar->next_variable = NULL;

    while(var->next_variable){
	var = var->next_variable;
	newvar->next_variable = (struct variable_list *)malloc(sizeof(struct variable_list));
	newvar = newvar->next_variable;
	memmove(newvar, var, sizeof(struct variable_list));
	if (var->name != NULL){
	    newvar->name = (oid *)malloc(var->name_length * sizeof(oid));
	    memmove(newvar->name, var->name, var->name_length * sizeof(oid));
	}
	if (var->val.string != NULL){
	    newvar->val.string = (u_char *)malloc(var->val_len);
	    memmove(newvar->val.string, var->val.string, var->val_len);
	}
	newvar->next_variable = 0;
    }
    return newpdu;
}

static oid risingAlarm[] = {1, 3, 6, 1, 6, 3, 2, 1, 1, 3, 1};
static oid fallingAlarm[] = {1, 3, 6, 1, 6, 3, 2, 1, 1, 3, 2};
static oid unavailableAlarm[] = {1, 3, 6, 1, 6, 3, 2, 1, 1, 3, 3};

void
event_input(struct variable_list *vp)
{
    int eventid;
    oid variable[MAX_OID_LEN];
    int variablelen;
    u_long destip;
    int sampletype;
    int value;
    int threshold;

    oid *op;

    vp = vp->next_variable;	/* skip sysUptime */
    if (vp->val_len != sizeof(risingAlarm)
	|| !memcmp(vp->val.objid, risingAlarm, sizeof(risingAlarm)))
	eventid = 1;
    else if (vp->val_len != sizeof(risingAlarm)
	|| !memcmp(vp->val.objid, fallingAlarm, sizeof(fallingAlarm)))
	eventid = 2;
    else if (vp->val_len != sizeof(risingAlarm)
	|| !memcmp(vp->val.objid, unavailableAlarm, sizeof(unavailableAlarm)))
	eventid = 3;
    else {
	fprintf(stderr, "unknown event\n");
	eventid = 0;
    }

    vp = vp->next_variable;
    memmove(variable, vp->val.objid, vp->val_len * sizeof(oid));
    variablelen = vp->val_len;
    op = vp->name + 22;
    destip = 0;
    destip |= (*op++) << 24;
    destip |= (*op++) << 16;
    destip |= (*op++) << 8;
    destip |= *op++;

    vp = vp->next_variable;
    sampletype = *vp->val.integer;
    
    vp = vp->next_variable;
    value= *vp->val.integer;
    
    vp = vp->next_variable;
    threshold = *vp->val.integer;
    
    printf("%d: 0x%02lX %d %d %d\n", eventid, destip, sampletype, value, threshold);
    
}

void do_external(char *cmd,
		 struct hostent *host,
		 struct snmp_pdu *pdu)
{
  struct variable_list tmpvar, *vars;
  struct sockaddr_in *pduIp = (struct sockaddr_in *)&(pdu->address);
  static oid trapoids[10] = {1,3,6,1,6,3,1,1,5};
  static oid snmpsysuptime[8] = {1,3,6,1,2,1,1,3};
  static oid snmptrapoid[10] = {1,3,6,1,6,3,1,1,4,1};
  static oid snmptrapent[10] = {1,3,6,1,6,3,1,1,4,3};
  int fd[2];
  int pid, result;
  FILE *file;
  char varbuf[SPRINT_MAX_LEN];
  int oldquick;
  
  DEBUGMSGTL(("snmptrapd", "Running: %s\n", cmd));
  oldquick = snmp_get_quick_print();
  snmp_set_quick_print(1);
  if (cmd) {
    if (pipe(fd)) {
      perror("pipe");
    }
    if ((pid = fork()) == 0) {
      /* child */
      close(0);
      if (dup(fd[0]) != 0) {
        perror("dup");
      }
      close(fd[1]);
      close(fd[0]);
      system(cmd);
      exit(0);
    } else if (pid > 0) {
      file = fdopen(fd[1],"w");
      fprintf(file,"%s\n%s\n",
              host ? host->h_name : inet_ntoa(pduIp->sin_addr),
              inet_ntoa(pduIp->sin_addr));
      if (pdu->command == SNMP_MSG_TRAP){
        /* convert a v1 trap to a v2 variable binding list:
           The uptime and trapOID go first in the list. */
        tmpvar.val.integer = (long *) &pdu->time;
        tmpvar.val_len = sizeof(pdu->time);
        tmpvar.type = ASN_TIMETICKS;
        sprint_variable(varbuf, snmpsysuptime, 8, &tmpvar);
        fprintf(file,"%s\n",varbuf);
        tmpvar.type = ASN_OBJECT_ID;
        trapoids[9] = pdu->trap_type+1;
        tmpvar.val.objid = trapoids;
        tmpvar.val_len = 10*sizeof(oid);
        sprint_variable(varbuf, snmptrapoid, 10, &tmpvar);
        fprintf(file,"%s\n",varbuf);
      }
      /* do the variables in the pdu */
      for(vars = pdu->variables; vars; vars = vars->next_variable) {
        sprint_variable(varbuf, vars->name, vars->name_length, vars);
        fprintf(file,"%s\n",varbuf);
      }
      if (pdu->command == SNMP_MSG_TRAP){
        /* convert a v1 trap to a v2 variable binding list:
           The enterprise goes last. */
        tmpvar.val.objid = pdu->enterprise;
        tmpvar.val_len = pdu->enterprise_length*sizeof(oid);
        sprint_variable(varbuf, snmptrapent, 10, &tmpvar);
        fprintf(file,"%s\n",varbuf);
      }
      fclose(file);
      close(fd[0]);
      close(fd[1]);
      if (waitpid(pid, &result,0) < 0) {
        perror("waitpid");
      }
    } else {
      perror("fork");
    }
  }
  snmp_set_quick_print(oldquick);
}

int snmp_input(int op,
	       struct snmp_session *session,
	       int reqid,
	       struct snmp_pdu *pdu,
	       void *magic)
{
    struct variable_list *vars;
    struct sockaddr_in *pduIp   = (struct sockaddr_in *)&(pdu->address);
    char buf[64], oid_buf [SPRINT_MAX_LEN], *cp;
    struct snmp_pdu *reply;
    struct tm *tm;
    time_t timer;
    struct hostent *host;
    int varbufidx;
    char varbuf[SPRINT_MAX_LEN];
    static oid trapoids[10] = {1,3,6,1,6,3,1,1,5};
    static oid snmptrapoid2[11] = {1,3,6,1,6,3,1,1,4,1,0};
    struct variable_list tmpvar;
    char *Command = NULL;
    tmpvar.type = ASN_OBJECT_ID;
                  
    if (op == RECEIVED_MESSAGE){
	if (pdu->command == SNMP_MSG_TRAP){
	    host = gethostbyaddr ((char *)&pduIp->sin_addr,
				  sizeof (pduIp->sin_addr), AF_INET);
	    if (Print && (pdu->trap_type != SNMP_TRAP_AUTHFAIL || dropauth == 0)) {
		time (&timer);
		tm = localtime (&timer);
                printf("%.4d-%.2d-%.2d %.2d:%.2d:%.2d %s [%s] %s:\n",
		       tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
		       tm->tm_hour, tm->tm_min, tm->tm_sec,
                       host ? host->h_name : inet_ntoa(pduIp->sin_addr),
                       inet_ntoa(pduIp->sin_addr),
 		       sprint_objid (oid_buf, pdu->enterprise, pdu->enterprise_length));
		if (pdu->trap_type == SNMP_TRAP_ENTERPRISESPECIFIC) {
		    oid trapOid[MAX_OID_LEN];
		    size_t trapOidLen = pdu->enterprise_length;
		    memcpy(trapOid, pdu->enterprise, sizeof(oid)*trapOidLen);
		    if (trapOid[trapOidLen-1] != 0) trapOid[trapOidLen++] = 0;
		    trapOid[trapOidLen++] = pdu->specific_type;
		    sprint_objid(oid_buf, trapOid, trapOidLen);
		    cp = strrchr(oid_buf, '.');
		    if (cp) cp++;
		    else cp = oid_buf;
		    printf("\t%s Trap (%s) Uptime: %s\n",
			   trap_description(pdu->trap_type), cp, 
			   uptime_string(pdu->time, buf));
		}
		else
		    printf("\t%s Trap (%ld) Uptime: %s\n",
			   trap_description(pdu->trap_type), pdu->specific_type,
			   uptime_string(pdu->time, buf));
		for(vars = pdu->variables; vars; vars = vars->next_variable) {
		    printf ("\t");
		    print_variable(vars->name, vars->name_length, vars);
		}
                printf("\n");
	    }
	    if (Syslog && (pdu->trap_type != SNMP_TRAP_AUTHFAIL || dropauth == 0)) {
	    	varbufidx=0;
	    	varbuf[varbufidx++]=','; varbuf[varbufidx++]=' ';
	    	varbuf[varbufidx]='\0';

	    	for(vars = pdu->variables; vars; vars = vars->next_variable) {
			sprint_variable(varbuf+varbufidx, vars->name,
                                        vars->name_length, vars);
			/* Update the length of the string with the
                           new variable */
			varbufidx += strlen(varbuf+varbufidx);
			/* And add a trailing , ... */
	        	varbuf[varbufidx++]=',';
	        	varbuf[varbufidx++]=' ';
	        	varbuf[varbufidx]='\0';
	    	}
	    	if ( varbufidx ) {
			varbufidx -= 2; varbuf[varbufidx]='\0';
	    	}
		if (pdu->trap_type == SNMP_TRAP_ENTERPRISESPECIFIC) {
		    oid trapOid[MAX_OID_LEN];
		    size_t trapOidLen = pdu->enterprise_length;
		    memcpy(trapOid, pdu->enterprise, sizeof(oid)*trapOidLen);
		    if (trapOid[trapOidLen-1] != 0) trapOid[trapOidLen++] = 0;
		    trapOid[trapOidLen++] = pdu->specific_type;
		    sprint_objid(oid_buf, trapOid, trapOidLen);
		    cp = strrchr(oid_buf, '.');
		    if (cp) cp++;
		    else cp = oid_buf;
		    syslog(LOG_WARNING, "%s: %s Trap (%s) Uptime: %s%s",
		       inet_ntoa(pduIp->sin_addr),
		       trap_description(pdu->trap_type), cp,
		       uptime_string(pdu->time, buf), varbuf);
		} else {
		    syslog(LOG_WARNING, "%s: %s Trap (%ld) Uptime: %s%s",
		       inet_ntoa(pduIp->sin_addr),
		       trap_description(pdu->trap_type), pdu->specific_type,
		       uptime_string(pdu->time, buf), varbuf);
		}
	    }
            trapoids[9] = pdu->trap_type+1;
            Command = snmptrapd_get_traphandler(trapoids, 10);
            if (Command)
              do_external(Command, host, pdu);
	} else if (pdu->command == SNMP_MSG_TRAP2
		   || pdu->command == SNMP_MSG_INFORM){
	    host = gethostbyaddr ((char *)&pduIp->sin_addr,
				  sizeof (pduIp->sin_addr), AF_INET);
	    if (Print) {
		time (&timer);
		tm = localtime (&timer);
                printf("%.4d-%.2d-%.2d %.2d:%.2d:%.2d %s [%s]:\n",
		       tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
		       tm->tm_hour, tm->tm_min, tm->tm_sec,
                       host ? host->h_name : inet_ntoa(pduIp->sin_addr),
                       inet_ntoa(pduIp->sin_addr));
		for (vars = pdu->variables; vars; vars = vars->next_variable) {
		    printf("\t");
		    print_variable(vars->name, vars->name_length, vars);
		}
		printf("\n");
	    }
	    if (Event) {
		event_input(pdu->variables);
	    }
            for(vars = pdu->variables;
                vars &&
                snmp_oid_compare(vars->name, vars->name_length, snmptrapoid2,
                         sizeof(snmptrapoid2)/sizeof(oid));
                vars = vars->next_variable);
            if (vars && vars->type == ASN_OBJECT_ID) {
              Command = snmptrapd_get_traphandler(vars->val.objid,
                                                  vars->val_len/sizeof(oid));
              if (Command)
                do_external(Command, host, pdu);
            }
	    if (pdu->command == SNMP_MSG_INFORM){
		if (!(reply = snmp_clone_pdu2(pdu, SNMP_MSG_RESPONSE))){
		    fprintf(stderr, "Couldn't clone PDU for response\n");
		    return 1;
		}
		reply->errstat = 0;
		reply->errindex = 0;
		reply->address = pdu->address;
		if (!snmp_send(session, reply)){
                    snmp_perror("snmptrapd: Couldn't respond to inform pdu");
		}
	    }
	}
    } else if (op == TIMED_OUT){
	fprintf(stderr, "Timeout: This shouldn't happen!\n");
    }
    return 1;
}


void usage(void)
{
    fprintf(stderr,"Usage: snmptrapd [-V] [-q] [-D] [-p #] [-P] [-s] [-f] [-l [d0-7]] [-e] [-d] [-H] [-S] [-a]\n");
}

RETSIGTYPE term_handler(int sig)
{
    running = 0;
}


int main(int argc, char *argv[])
{
    struct snmp_session sess, *session = &sess, *ss;
    struct usmUser *user, *userListPtr;
    int	arg;
    int count, numfds, block;
    fd_set fdset;
    struct timeval timeout, *tvp;
    int local_port = SNMP_TRAP_PORT;
    int dofork=1;

#ifdef notused
    in_addr_t myaddr;
    oid src[MAX_OID_LEN], dst[MAX_OID_LEN], context[MAX_OID_LEN];
    int srclen, dstlen, contextlen;
    char ctmp[300];
#endif

    /* register our configuration handlers now so -H properly displays them */
    register_config_handler("snmptrapd","traphandle",snmptrapd_traphandle,NULL,"oid program [args ...] ");

    setvbuf (stdout, NULL, _IOLBF, BUFSIZ);
    /*
     * usage: snmptrapd [-q] [-D] [-p #] [-P] [-s] [-f] [-l [d0-7]] [-d] [-e] [-S] [-a]
     */
    for(arg = 1; arg < argc; arg++){
	if (argv[arg][0] == '-'){
	    switch(argv[arg][1]){
		case 'V':
                  fprintf(stderr,"UCD-snmp version: %s\n", VersionInfo);
                  exit(0);
                  break;
		case 'd':
		    snmp_set_dump_packet(1);
		    break;
		case 'q':
		    snmp_set_quick_print(1);
		    break;
		case 'D':
                    debug_register_tokens(&argv[arg][2]);
                    snmp_set_do_debugging(1);
                    break;
                case 'p':
		    if (++arg == argc) {
			usage();
			exit(1);
		    }
                    local_port = atoi(argv[arg]);
                    break;
		case 'm':
		    if (argv[arg][2] != 0)
			setenv("MIBS",&argv[arg][2], 1);
		    else if (++arg < argc)
			setenv("MIBS",argv[arg], 1);
		    else {
			fprintf(stderr,"Need MIBS after -m flag.\n");
			usage();
			exit(1);
		    }
		    break;
		case 'M':
		    if (argv[arg][2] != 0)
			setenv("MIBDIRS",&argv[arg][2], 1);
		    else if (++arg < argc)
			setenv("MIBDIRS",argv[arg], 1);
		    else {
			fprintf(stderr,"Need MIBDIRS after -M flag.\n");
			usage();
			exit(1);
		    }
		    break;
		case 'P':
		    Print++;
		    break;
		case 'e':
		    Event++;
		    break;
		case 's':
		    Syslog++;
		    break;
		case 'S':
		    snmp_set_suffix_only(2);
		    break;
		case 'a':
		    dropauth = 1;
		    break;
                case 'f':
		    dofork = 0;
		    break;
		case 'l':
		    arg++;
		    switch(argv[arg][0]) {
			case 'd':
			   Facility = LOG_DAEMON; break;
			case '0':
			   Facility = LOG_LOCAL0; break;
			case '1':
			   Facility = LOG_LOCAL1; break;
			case '2':
			   Facility = LOG_LOCAL2; break;
			case '3':
			   Facility = LOG_LOCAL3; break;
			case '4':
			   Facility = LOG_LOCAL4; break;
			case '5':
			   Facility = LOG_LOCAL5; break;
			case '6':
			   Facility = LOG_LOCAL6; break;
			case '7':
			   Facility = LOG_LOCAL7; break;
			default:
		    	   fprintf(stderr,"invalid  syslog facility: -l %c\n",
							argv[arg][0]);
		    	   usage();
		    	   exit (1);
		           break;
		    }
		    break;
                case 'H':
                    init_snmp("snmptrapd");
                    fprintf(stderr, "Configuration directives understood:\n");
                    read_config_print_usage("  ");
                    exit(0);
		default:
		    fprintf(stderr,"invalid option: -%c\n", argv[arg][1]);
		    usage();
		    exit (1);
		    break;
	    }
	    continue;
	}
	else {
	    usage();
	    exit (1);
	}
    }

    if (!Print) Syslog = 1;
    
    register_config_handler("snmptrapd","traphandle",snmptrapd_traphandle,NULL,"script");

    /* Initialize the world. Create initial user */
    usm_set_reportErrorOnUnknownID(1);
    init_snmpv3("snmptrapd");	/* register the v3 handlers */

    register_mib_handlers();/* snmplib .conf handlers */
    read_premib_configs();	/* read pre-mib-reading .conf handlers */

#ifdef TESTING
    print_config_handlers();
#endif

    /* create the initial and template users */
    user = usm_create_initial_user("initial", usmHMACMD5AuthProtocol,
				   USM_LENGTH_OID_TRANSFORM,
				   usmDESPrivProtocol,
				   USM_LENGTH_OID_TRANSFORM);
    userListPtr = usm_add_user(user);
    user = usm_create_initial_user("templateMD5", usmHMACMD5AuthProtocol,
				   USM_LENGTH_OID_TRANSFORM,
				   usmDESPrivProtocol,
				   USM_LENGTH_OID_TRANSFORM);
    userListPtr = usm_add_user(user);
    user = usm_create_initial_user("templateSHA", usmHMACSHA1AuthProtocol,
				   USM_LENGTH_OID_TRANSFORM,
				   usmDESPrivProtocol,
				   USM_LENGTH_OID_TRANSFORM);
    userListPtr = usm_add_user(user);
        
    if (userListPtr == NULL) /* user already existed */
      usm_free_user(user);

    update_config(0);	/* read in config files and register HUP */

#if 0

    sprintf(ctmp,"%s/party.conf",SNMPSHAREPATH);
    if (read_party_database(ctmp) != 0){
	fprintf(stderr, "Warning: Couldn't read v2party database from %s\n",ctmp);
    }
    sprintf(ctmp,"%s/context.conf",SNMPSHAREPATH);
    if (read_context_database(ctmp) != 0){
	fprintf(stderr, "Warning: Couldn't read v2party's context database from %s\n",ctmp);
    }
    sprintf(ctmp,"%s/acl.conf",SNMPSHAREPATH);
    if (read_acl_database(ctmp) != 0){
	fprintf(stderr,
		"Warning: Couldn't read v2party's access control database from %s\n",ctmp);
    }

#endif



    /* fork the process to the background if we are not printing to stdout */
    if (!Print && dofork) {
      int fd, fdnum;

      switch (fork()) {
	case -1:
		fprintf(stderr,"bad fork - %s\n",strerror(errno));
		_exit(1);

	case 0:
		/* become process group leader */
		if (setsid() == -1) {
			fprintf(stderr,"bad setsid - %s\n",strerror(errno));
			_exit(1);
		}

        /* if we are forked, we don't want to print out to stdout or stderr */
		fd=open("/dev/null", O_RDWR);
		dup2(fd, STDIN_FILENO);
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDERR_FILENO);
		close(fd);
                /* Close all unnecessary file descriptors */
		fdnum = getdtablesize();
		for ( fd = (STDERR_FILENO + 1); fd < fdnum; fd++ )
			close(fd);
		break;

	default:
		_exit(0);
      }
    }

    if (Syslog) {
      /* open syslog */
      init_syslog();
    }
    if (Print) {
      struct tm *tm;
      time_t timer;
      time (&timer);
      tm = localtime (&timer);
      printf("%.4d-%.2d-%.2d %.2d:%.2d:%.2d UCD-snmp version %s Started.\n",
             tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
             tm->tm_hour, tm->tm_min, tm->tm_sec,
             VersionInfo);
    }


    memset(session, 0, sizeof(struct snmp_session));
    session->peername = SNMP_DEFAULT_PEERNAME; /* Original code had NULL here */
    session->version = SNMP_DEFAULT_VERSION;

    session->srcPartyLen = 0;
    session->dstPartyLen = 0;
    session->contextLen = 0; 

    session->community_len = SNMP_DEFAULT_COMMUNITY_LEN;

    session->retries = SNMP_DEFAULT_RETRIES;
    session->timeout = SNMP_DEFAULT_TIMEOUT;
     
    session->local_port = local_port;

    session->callback = snmp_input; 
    session->callback_magic = NULL; 
    session->authenticator = NULL;

    ss = snmp_open( session );
    if (ss == NULL){
        snmp_perror("snmptrapd");
        if (Syslog) {
	    syslog(LOG_ERR,"couldn't open snmp - %m");
	}
	exit(1);
    }

    signal(SIGTERM, term_handler);
    signal(SIGHUP, term_handler);
    signal(SIGINT, term_handler);

    while (running) {
	numfds = 0;
	FD_ZERO(&fdset);
	block = 0;
	tvp = &timeout;
	timerclear(tvp);
	tvp->tv_sec = 5;
	snmp_select_info(&numfds, &fdset, tvp, &block);
	if (block == 1)
	    tvp = NULL;	/* block without timeout */
	count = select(numfds, &fdset, 0, 0, tvp);
	gettimeofday(&Now, 0);
	if (count > 0){
	    snmp_read(&fdset);
	} else switch(count){
	    case 0:
		snmp_timeout();
		break;
	    case -1:
	        perror("select");
		running = 0;
	    default:
		fprintf(stderr, "select returned %d\n", count);
		running = 0;
	}
    }

    if (Print) {
	struct tm *tm;
	time_t timer;
	time (&timer);
	tm = localtime (&timer);
	printf("%.4d-%.2d-%.2d %.2d:%.2d:%.2d UCD-snmp version %s Stopped.\n",
	       tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
	       tm->tm_hour, tm->tm_min, tm->tm_sec,
	       VersionInfo);
    }
    return 0;
}

void
init_syslog(void)
{
    /*
     * All messages will be logged to the local0 facility and will be sent to
     * the console if syslog doesn't work.
     */
    openlog("snmptrapd", LOG_CONS|LOG_PID, Facility);
    syslog(LOG_INFO, "Starting snmptrapd");
}


/*
 * Read the configuration files. Implemented as a signal handler so that
 * receipt of SIGHUP will cause configuration to be re-read when the
 * trap deamon is running detatched from the console.
 *
 */
void update_config(int a) 
{
#if 0  
  if (!dontReadConfigFiles) {  /* don't read if -C present on command line */
#endif

    read_configs();

#if 0
  }
#endif
  
  /* read all optional config files */
  /* last is -c from command line */
  /* always read this one even if -C is present (ie both -c and -C) */

#if 0
  if (optconfigfile != NULL) {
    read_config_with_type (optconfigfile, "snmptrapd");
  } 
#endif
  snmp_call_callbacks(SNMP_CALLBACK_LIBRARY, SNMP_CALLBACK_POST_READ_CONFIG,
                      NULL);

  signal(SIGHUP, update_config);
}


#ifndef HAVE_GETDTABLESIZE
#include <sys/resource.h>
int getdtablesize(void)
{
  struct rlimit rl;
  getrlimit(RLIMIT_NOFILE, &rl);
  return( rl.rlim_cur );
}
#endif
