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

#define MAX_EVENT_NUMBER 1024
#define TCP_BUFFER_SIZE 512
#define UDP_BUFFER_SIZE 1024

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
   event.events = EPOLLIN | EPOLLET;
   epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
   setnonblocking(fd);
}

int main(int argc, char* argv[])
{
   printf("[Server] Test demo - tcp_udp\n");

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

   int ret = 0;
   // 创建TCP socket, 绑定到port上
   int listenfd = socket(PF_INET, SOCK_STREAM, 0);
   printf("create listenfd %d\n", listenfd);
   assert(listenfd >=0);
   int reuse = 1;
   ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
   assert(ret != -1);

   ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
   assert(ret != -1);

   ret = listen(listenfd, 5);
   assert(ret != -1);

   // 创建UDP socket，绑定到port上
   bzero(&address, sizeof(address));
   address.sin_family = AF_INET;
   inet_pton(AF_INET, ip, &address.sin_addr);
   address.sin_port = htons(port);
   int udpfd = socket(PF_INET, SOCK_DGRAM, 0);
   printf("create udpfd %d\n", udpfd);
   assert(udpfd > 0);

   ret = bind(udpfd, (struct sockaddr*)&address, sizeof(address));
   assert(ret != -1);

   epoll_event events[MAX_EVENT_NUMBER];
   int epollfd = epoll_create1(0);
   assert(epollfd != -1);
   addfd(epollfd, listenfd);
   addfd(epollfd, udpfd);

   while (1)
   {
      int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
      if (number < 0)
      {
         printf("epoll failure\n");
         break;
      }

      for (int i = 0; i < number; i++)
      {
         int sockfd = events[i].data.fd;
         if (sockfd == listenfd)
         {
            struct sockaddr_in client_address;
            socklen_t client_addrlength = sizeof(client_address);
            int connfd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlength);
            addfd(epollfd, connfd);
            printf("comes a tcp connection %d from ip: %s, port: %d\n", connfd, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
         }
         else if (sockfd == udpfd)
         {
            char buf[UDP_BUFFER_SIZE];
            memset(buf, '\0', UDP_BUFFER_SIZE);
            struct sockaddr_in client_address;
            socklen_t client_addrlength = sizeof(client_address);

            ret = recvfrom(udpfd, buf, UDP_BUFFER_SIZE-1, 0, (struct sockaddr*)&client_address, &client_addrlength);
            printf("comes udp data from ip: %s, port: %d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
            if (ret > 0)
            {
               sendto(udpfd, buf, UDP_BUFFER_SIZE-1, 0, (struct sockaddr*)&client_address, client_addrlength);
            }
         }
         else if (events[i].events & EPOLLIN)
         {
            char buf[TCP_BUFFER_SIZE];
            while (1)
            {
               memset(buf, '\0', TCP_BUFFER_SIZE);
               ret = recv(sockfd, buf, TCP_BUFFER_SIZE-1, 0);
               if (ret < 0)
               {
                  if ((errno = EAGAIN) || (errno == EWOULDBLOCK))
                  {
                     // no data
                     break;
                  }
                  close(sockfd);
                  break;
               }
               else if (ret == 0)
               {
                  printf("connect %d is closed\n", sockfd);
                  close(sockfd);
               }
               else
               {
                  send(sockfd, buf, ret, 0);
               }
            }
         }
         else
         {
            printf("something else happened\n");
         }
      }
   }

   printf("Stop the server\n");
   close(listenfd);
   close(udpfd);

   return 0;
}

