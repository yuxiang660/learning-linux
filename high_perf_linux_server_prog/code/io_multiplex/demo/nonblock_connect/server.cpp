#include <stdio.h>
#include <libgen.h> // basename
#include <stdlib.h> // atoi
#include <assert.h>
#include <string.h> // bzero
#include <unistd.h> // sleep, close
#include <errno.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define MAXEVENTS 64

int setnonblocking(int fd)
{
   int old_option = fcntl(fd, F_GETFL);
   int new_option = old_option | O_NONBLOCK;
   fcntl(fd, F_SETFL, new_option);
   return old_option;
}

void addfd(int epollfd, int fd)
{
   epoll_event event;
   event.data.fd = fd;
   event.events = EPOLLIN;
   epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
   setnonblocking(fd);
}

int main(int argc, char* argv[])
{
   printf("[Server] Test demo - nonblock connect\n");

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
   printf("create listenfd %d\n", listenfd);
   assert(listenfd >=0);

   int ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
   assert(ret != -1);

   ret = listen(listenfd, 5);
   assert(ret != -1);

   int epoll_fd = epoll_create1(0);
   assert(epoll_fd > 0);
   addfd(epoll_fd, listenfd);

   epoll_event events[MAXEVENTS];

   bool stop = false;
   while (!stop)
   {
      int event_count = epoll_wait(epoll_fd, events, MAXEVENTS, -1/*infinite timeout*/);
      if (event_count < 0)
      {
         printf("epoll failure\n");
         break;
      }

      for (int i = 0; i < event_count; i++)
      {
         printf("recevie event %u from file descriptor %d\n", events[i].events, events[i].data.fd);

         if (events[i].data.fd == listenfd)
         {
            struct sockaddr_in client_address;
            socklen_t client_addrlength = sizeof(client_address);
            int connfd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlength);
            printf("connfd %d is create for ip: %s, port: %d\n", connfd, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
            // print log right after connection
            fflush(stdout);
            close(connfd);
         }
      }
   }

   printf("Stop the server\n");
   close(epoll_fd);
   close(listenfd);

   return 0;
}

