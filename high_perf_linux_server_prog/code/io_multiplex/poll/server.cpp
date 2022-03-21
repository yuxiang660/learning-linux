#include <stdio.h>
#include <libgen.h> // basename
#include <stdlib.h> // atoi
#include <assert.h>
#include <string.h> // bzero
#include <unistd.h> // sleep, close
#include <errno.h>
#include <poll.h>

#include <arpa/inet.h>

int main(int argc, char* argv[])
{
   printf("[Server] Test select\n");

   if (argc <= 2)
   {
      printf("usage: %s ip_address port_number\n", basename(argv[0]));
      return 1;
   }

   const char *ip = argv[1];
   int port = atoi(argv[2]);

   struct sockaddr_in address;
   bzero(&address, sizeof(address));
   address.sin_family = AF_INET;
   inet_pton(AF_INET, ip, &address.sin_addr);
   address.sin_port = htons(port);

   int listenfd = socket(PF_INET, SOCK_STREAM, 0);
   assert(listenfd >=0);

   int ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
   assert(ret != -1);

   ret = listen(listenfd, 5);
   assert(ret != -1);

   struct sockaddr_in client;
   socklen_t client_addrlength = sizeof(client);
   int connfd = accept(listenfd, (struct sockaddr*)&client, &client_addrlength);
   if (connfd < 0)
   {
      printf("error is: %d\n", errno);
      close(listenfd);
      return 1;
   }

   char buf[1024];
   struct pollfd poll_set[1];
   poll_set[0].fd = connfd;
   poll_set[0].events = POLLIN | POLLPRI /* urgent data */;

   while (1)
   {
      ret = poll(poll_set, 1, -1/*infinite timeout*/);
      if (ret < 0)
      {
         printf("poll failure\n");
         break;
      }

      printf("receive event %d\n", poll_set[0].revents);

      if (poll_set[0].revents & POLLIN)
      {
         memset(buf, '\0', sizeof(buf));
         ret = recv(poll_set[0].fd, buf, sizeof(buf) - 1, 0);
         if (ret < 0)
         {
            printf("fail to read for normal data\n");
            break;
         }
         if (ret == 0)
         {
            printf("the socket is closed\n");
            break;
         }
         printf("get %d bytes of normal data: %s\n", ret, buf);
      }

      if (poll_set[0].revents & POLLPRI)
      {
         memset(buf, '\0', sizeof(buf));
         ret = recv(poll_set[0].fd, buf, sizeof(buf) - 1, MSG_OOB);
         if (ret <= 0)
         {
            printf("fail to read for oob data\n");
            break;
         }
         printf("get %d bytes of obb data: %s\n", ret, buf);
      }
   }

   printf("Stop the server\n");
   close(connfd);
   close(listenfd);

   return 0;
}

