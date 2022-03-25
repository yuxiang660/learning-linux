#include <stdio.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define BUFFER_SIZE 65537

int main(int argc, char* argv[])
{
   printf("[Server] Test connect timeout\n");

   if (argc <= 2)
   {
      printf("usage: %s ip_address port_number\n", basename(argv[0]));
      return 1;
   }

   const char* ip = argv[1];
   int port = atoi(argv[2]);

   struct sockaddr_in address;
   bzero(&address, sizeof(address));
   address.sin_family = AF_INET;
   inet_pton(AF_INET, ip, &address.sin_addr);
   address.sin_port = htons(port);

   int sock = socket(PF_INET, SOCK_STREAM, 0);
   assert(sock >= 0);

   int ret = 0;
   // workaround TIME_WAIT issue
   int reuse = 1;
   ret = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
   assert(ret != -1);

   ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
   if (ret < 0)
   {
      printf("bind failure, err=%d, msg=%s\n", errno, strerror(errno));
      return 1;
   }

   ret = listen(sock, 5);
   assert(ret != -1);

   struct sockaddr_in client;
   socklen_t client_len = sizeof(client);
   int connfd = accept(sock, (struct sockaddr*)&client, &client_len);
   if (connfd < 0)
   {
      printf("error is %d\n", errno);
   }

   printf("connfd %d connected\n", connfd);
   close(connfd);
   close(sock);
   return 0;
}
