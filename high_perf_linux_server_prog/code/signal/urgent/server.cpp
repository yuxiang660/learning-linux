#include <stdio.h>
#include <libgen.h> // basename
#include <stdlib.h> // atoi
#include <assert.h>
#include <string.h> // bzero
#include <unistd.h> // sleep, close
#include <errno.h>
#include <signal.h>
#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>

#define BUF_SIZE 1024

static int connfd;
void sig_urg(int sig)
{
   int save_error = errno;
   char buffer[BUF_SIZE];
   memset(buffer, '\0', BUF_SIZE);
   int ret = recv(connfd, buffer, BUF_SIZE-1, MSG_OOB); // 接收带外数据
   printf("got %d bytes of oob data '%s'\n", ret, buffer);
   errno = save_error;
}

void addsig(int sig, void(*sig_handler)(int))
{
   struct sigaction sa;
   memset(&sa, '\0', sizeof(sa));
   sa.sa_handler = sig_handler;
   sa.sa_flags |= SA_RESTART;
   sigemptyset(&sa.sa_mask);
   assert(sigaction(sig, &sa, NULL) != -1);
}

int main(int argc, char* argv[])
{
   printf("[Server] Test SIGURG\n");

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
   connfd = accept(listenfd, (struct sockaddr*)&client, &client_addrlength);
   if (connfd < 0)
   {
      printf("error is: %d\n", errno);
      close(listenfd);
      return 1;
   }

   addsig(SIGURG, sig_urg);
   //使用SIGURG信号之前，我们必须设置socket的宿主进程或进程组
   fcntl(connfd, F_SETOWN, getpid());

   char buffer[1024];
   while (1)
   {
      memset(buffer, '\0', BUF_SIZE);
      ret = recv(connfd, buffer, BUF_SIZE-1, 0);
      if (ret == 0)
      {
         printf("connection is closed\n");
         break;
      }
      else if (ret < 0)
      {
         printf("recv failure\n");
         break;
      }
      printf("got %d bytes of normal data '%s'\n", ret, buffer);
   }

   printf("Stop the server\n");
   close(connfd);
   close(listenfd);

   return 0;
}

