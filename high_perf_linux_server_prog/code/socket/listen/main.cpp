#include <stdio.h>
#include <libgen.h> // basename
#include <stdlib.h> // atoi
#include <assert.h>
#include <string.h> // bzero
#include <unistd.h> // sleep, close

#include <arpa/inet.h>

#include <signal.h>

static bool stop = false;
static void handle_term(int sig)
{
   stop = true;
}

int main(int argc, char* argv[])
{
   printf("Test listen backlog\n");

   signal(SIGTERM, handle_term);

   if (argc <= 3)
   {
      printf("usage: %s ip_address port_number backlog\n", basename(argv[0]));
   }

   const char *ip = argv[1];
   int port = atoi(argv[2]);
   int backlog = atoi(argv[3]);

   int sock = socket(PF_INET, SOCK_STREAM, 0);
   assert(sock >=0);

   struct sockaddr_in address;
   bzero(&address, sizeof(address));
   address.sin_family = AF_INET;
   inet_pton(AF_INET, ip, &address.sin_addr);
   address.sin_port = htons(port);

   int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
   assert(ret != -1);

   ret = listen(sock, backlog);
   assert(ret != -1);

   while (!stop)
   {
      sleep(1);
   }

   printf("Stop the server\n");
   close(sock);

   return 0;
}

