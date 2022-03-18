#include <netdb.h>
#include <stdio.h>
#include <libgen.h> // basename
#include <assert.h>
#include <unistd.h> // sleep, close

#include <arpa/inet.h>

int main(int argc, char* argv[])
{
   printf("Test getservbyname\n");

   if (argc <= 1)
   {
      printf("usage: %s ip_address\n", basename(argv[0]));
      return 1;
   }

   const char *host = argv[1];
   hostent *hostinfo = gethostbyname(host);
   assert(hostinfo);

   servent *servinfo = getservbyname("ssh", "tcp");
   assert(servinfo);
   printf("ssh port is %d\n", ntohs(servinfo->s_port));

   return 0;
}

