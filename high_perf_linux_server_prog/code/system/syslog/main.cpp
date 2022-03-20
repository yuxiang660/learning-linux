#include <stdio.h>
#include <syslog.h>

// Add log in "/var/log/syslog"
//    Mar 20 15:00:15 cadence-vm Logs[599912]: Start loggine
int main()
{
   printf("Test syslog\n");

   openlog("Logs", LOG_PID, LOG_USER);
   syslog(LOG_INFO, "Start loggine");
   closelog();

   return 0;
}
