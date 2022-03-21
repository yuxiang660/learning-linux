#include <stdio.h>
#include <libgen.h> // basename
#include <stdlib.h> // atoi
#include <assert.h>
#include <string.h> // bzero
#include <unistd.h> // sleep, close
#include <errno.h>
#include <sys/epoll.h>

#include <arpa/inet.h>

#define MAXEVENTS 5

int main(int argc, char* argv[])
{
   printf("[Server] Test epoll\n");

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
   else
   {
      printf("connfd %d is connected\n", connfd);
   }

   char buf[1024];
   int epoll_fd = epoll_create1(0);
   assert(epoll_fd > 0);
   struct epoll_event event, events[MAXEVENTS];;
   event.events = EPOLLIN | EPOLLPRI /* urgent data */;
   event.data.fd = connfd;

   ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, connfd, &event);
   assert(ret != -1);

   bool stop = false;
   while (!stop)
   {
      int event_count = epoll_wait(epoll_fd, events, MAXEVENTS, -1/*infinite timeout*/);
      if (event_count < 0)
      {
         printf("epoll failure\n");
         break;
      }
      else if (event_count == 0)
      {
         printf("no event happened\n");
         continue;
      }

      printf("receive %d events\n", event_count);
      for (int i = 0; i < event_count; i++)
      {
         printf("recevie event %u from file descriptor %d\n", events[i].events, events[i].data.fd);

         if (events[i].events & EPOLLPRI)
         {
            memset(buf, '\0', sizeof(buf));
            ret = recv(events[i].data.fd, buf, sizeof(buf) - 1, MSG_OOB);
            if (ret <= 0)
            {
               printf("fail to read for oob data\n");
               stop = true;
               break;
            }
            printf("get %d bytes of obb data: %s\n", ret, buf);
         }

         if (events[i].events & EPOLLIN)
         {
            memset(buf, '\0', sizeof(buf));
            ret = recv(events[i].data.fd, buf, sizeof(buf) - 1, 0);
            if (ret < 0)
            {
               printf("fail to read for normal data\n");
               break;
            }
            if (ret == 0)
            {
               printf("the socket is closed\n");
               stop = true;
               break;
            }
            printf("get %d bytes of normal data: %s\n", ret, buf);
         }
      }
   }

   printf("Stop the server\n");
   close(epoll_fd);
   close(connfd);
   close(listenfd);

   return 0;
}

