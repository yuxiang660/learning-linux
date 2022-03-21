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
   fd_set read_fds;
   fd_set exception_fds;
   FD_ZERO(&read_fds);
   FD_ZERO(&exception_fds);

   while (1)
   {
      memset(buf, '\0', sizeof(buf));
      // 每次调用select前都要重新在read_fds和exception_fds中设置文件描述符connfd，因为事件发生之后，文件描述符将被内核修改
      FD_SET(connfd, &read_fds);
      FD_SET(connfd, &exception_fds);
      ret = select(connfd + 1, &read_fds, NULL, &exception_fds, NULL);
      if (ret < 0)
      {
         printf("selection failure\n");
         break;
      }
      // 对于可读事件，采用普通的recv函数读取数据
      if (FD_ISSET(connfd, &read_fds))
      {
         ret = recv(connfd, buf, sizeof(buf) - 1, 0);
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
      // 对于异常事件，采用带MSG_OBB标志的recv函数读取带外数据
      else if (FD_ISSET(connfd, &exception_fds))
      {
         ret = recv(connfd, buf, sizeof(buf) - 1, MSG_OOB);
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

