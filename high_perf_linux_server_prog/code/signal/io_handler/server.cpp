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
#include <signal.h>

#define MAX_EVENT_NUMBER 1024
static int pipefd[2];

int setnonblocking(int fd)
{
   int old_option = fcntl(fd, F_GETFL);
   int new_option = old_option | O_NONBLOCK;
   assert(fcntl(fd, F_SETFL, new_option) != -1);
   return old_option;
}

void addfd(int epollfd, int fd)
{
   epoll_event event;
   event.data.fd = fd;
   event.events = EPOLLIN | EPOLLET;
   assert(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event) != -1);
   setnonblocking(fd);
}

void sig_handler(int sig)
{
   // 保留原来的errno，在函数最后恢复，以保证函数的可重入性
   int save_errno = errno;
   int msg = sig;
   send(pipefd[1], reinterpret_cast<char*>(&msg), 1, 0); //将信号值写入管道，以通知循环
   errno = save_errno;
}

void addsig(int sig)
{
   struct sigaction new_action;
   memset(&new_action, '\0', sizeof(new_action));
   new_action.sa_handler = sig_handler;
   // sigaction mask is not functional, if you want to mask any signal, please use sigprocmask, refer to ../mask/main.cpp
   sigemptyset(&new_action.sa_mask);
   new_action.sa_flags |= SA_RESTART;
   assert(sigaction(sig, &new_action, NULL) != -1);
}

int main(int argc, char* argv[])
{
   printf("[Server] Test signal\n");

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

   epoll_event events[MAX_EVENT_NUMBER];
   int epollfd = epoll_create1(0);
   assert(epollfd != -1);
   addfd(epollfd, listenfd);

   // 使用socketpair创建双向管道
   ret = socketpair(PF_UNIX, SOCK_STREAM, 0, pipefd);
   //ret = pipe(pipefd);
   assert(ret != -1);
   setnonblocking(pipefd[1]);
   addfd(epollfd, pipefd[0]);

   addsig(SIGHUP);
   addsig(SIGCHLD);
   addsig(SIGTERM);
   addsig(SIGINT);
   bool stop_server = false;

   while (!stop_server)
   {
      int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
      if (number < 0 && errno != EINTR)
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
         }
         else if (sockfd == pipefd[0] && events[i].events & EPOLLIN)
         {
            int sig;
            char signals[1024];
            int number_of_sigs = recv(pipefd[0], signals, sizeof(signals), 0);
            if (number_of_sigs == -1)
            {
               printf("fail to get sig\n");
               continue;
            }
            else if (number_of_sigs == 0)
            {
               printf("no sig is got\n");
               continue;
            }
            else
            {
               for (int i = 0; i < number_of_sigs; i++)
               {
                  printf("rececive sig number %d\n", signals[i]);
                  switch (signals[i])
                  {
                  case SIGCHLD:
                  case SIGHUP:
                  {
                     printf("do nothing\n");
                     break;
                  }
                  case SIGTERM:
                  case SIGINT:
                  {
                     stop_server = true;
                     break;
                  }
                  default:
                  {
                     printf("unexpected signal\n");
                  }
                  }
               }
            }
         }
         else
         {
            printf("unexpected event\n");
         }
      }
   }

   printf("Stop the server\n");
   close(listenfd);
   close(pipefd[0]);
   close(pipefd[1]);
   return 0;
}

