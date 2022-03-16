#include <stdio.h>
#include <libgen.h> // basename
#include <stdlib.h> // atoi
#include <assert.h>
#include <string.h> // bzero
#include <unistd.h> // sleep, close
#include <errno.h>

#include <arpa/inet.h>

int main(int argc, char* argv[])
{
   printf("Test accept\n");

   if (argc <= 2)
   {
      printf("usage: %s ip_address port_number\n", basename(argv[0]));
      return 1;
   }

   const char *ip = argv[1];
   int port = atoi(argv[2]);

   int sock = socket(PF_INET, SOCK_STREAM, 0);
   assert(sock >=0);

   struct sockaddr_in address;
   bzero(&address, sizeof(address));
   address.sin_family = AF_INET;
   inet_pton(AF_INET, ip, &address.sin_addr);
   address.sin_port = htons(port);

   int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
   assert(ret != -1);

   ret = listen(sock, 5);
   assert(ret != -1);

   struct sockaddr_in client;
   socklen_t client_addrlength = sizeof(client);
   int connfd = accept(sock, (struct sockaddr*)&client, &client_addrlength);
   if (connfd < 0)
   {
      printf("error is: %d\n", errno);
   }
   else
   {
      char remote[INET_ADDRSTRLEN];
      printf("connected with ip: %s and port: %d\n",
         inet_ntop(AF_INET, &client.sin_addr, remote, INET_ADDRSTRLEN),
         ntohs(client.sin_port));
      close(connfd);
   }

   printf("Stop the server\n");
   close(sock);

   return 0;
}

