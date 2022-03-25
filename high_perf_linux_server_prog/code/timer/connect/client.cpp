#include <stdio.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int timeout_connect(const char* ip, int port, int time)
{
   int ret = 0;
   struct sockaddr_in address;
   bzero(&address, sizeof(address));
   address.sin_family = AF_INET;
   inet_pton(AF_INET, ip, &address.sin_addr);
   address.sin_port = htons(port);

   int sockfd = socket(PF_INET, SOCK_STREAM, 0);
   assert(sockfd > 0);
   // 通过选项SO_RCVTIMEO和SO_SNDTIMEO所设置的超时时间的类型是timeval，这个和select系统调用的超时参数类型相同
   struct timeval timeout;
   timeout.tv_sec = time;
   timeout.tv_usec = 0;
   socklen_t len = sizeof(timeout);
   ret = setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeout, len);
   assert(ret != -1);
   ret = connect(sockfd, (struct sockaddr*)&address, sizeof(address));
   if (ret == -1)
   {
      // 超时对应的错误是EINPROGRESS
      if (errno == EINPROGRESS)
      {
         printf("connecting timeout, process timeout logic\n");
         return -1;
      }
      printf("error occur when connecting to server, err=%d, msg=%s\n", errno, strerror(errno));
      return -1;
   }
   return sockfd;
}

int main(int argc, char* argv[])
{
   printf("[Client] Test connect timeout\n");

   if (argc <= 2)
   {
      printf("usage: %s ip_address port_number n", basename(argv[0]));
      return 1;
   }

   const char* ip = argv[1];
   int port = atoi(argv[2]);

   int sock = timeout_connect(ip, port, 10);
   if (sock < 0)
   {
      return 1;
   }

   printf("client connects to server successfully\n");
   close(sock);
   return 0;
}
