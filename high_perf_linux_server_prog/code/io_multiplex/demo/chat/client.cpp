#include <stdio.h>
#include <libgen.h> // basename
#include <stdlib.h> // atoi
#include <assert.h>
#include <string.h> // bzero
#include <unistd.h> // sleep, close
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>

#define BUFFER_SIZE 100

int main(int argc, char* argv[])
{
   printf("[Client] Test demo - chat\n");

   if (argc <= 2)
   {
      printf("usage: %s ip_address port_number\n", basename(argv[0]));
      return 1;
   }

   const char *ip = argv[1];
   int port = atoi(argv[2]);

   struct sockaddr_in server_address;
   bzero(&server_address, sizeof(server_address));
   server_address.sin_family = AF_INET;
   inet_pton(AF_INET, ip, &server_address.sin_addr);
   server_address.sin_port = htons(port);

   int sockfd = socket(PF_INET, SOCK_STREAM, 0);
   assert(sockfd > 0);
   if (connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
   {
      printf("connection failed\n");
      close(sockfd);
      return 1;
   }

   pollfd fds[2];

   fds[0].fd = STDIN_FILENO;
   fds[0].events = POLLIN;
   fds[0].revents = 0;
   fds[1].fd = sockfd;
   fds[1].events = POLLIN | POLLRDHUP /* the other end of the socket closed */;
   fds[1].revents = 0;

   char read_buf[BUFFER_SIZE];
   int pipefd[2];
   int ret = pipe(pipefd);
   assert(ret != -1);

   while (1)
   {
      ret = poll(fds, 2, -1);
      if (ret < 0)
      {
         printf("poll failure\n");
         break;
      }

      if (fds[1].revents & POLLRDHUP)
      {
         printf("server close the connection\n");
         break;
      }
      else if (fds[1].revents & POLLIN)
      {
         memset(read_buf, '\0', BUFFER_SIZE);
         recv(fds[1].fd, read_buf, BUFFER_SIZE-1, 0);
         printf("%s\n", read_buf);
      }

      if (fds[0].revents & POLLIN)
      {
         // 使用splice将用户输入的数据直接写到sockfd上(零拷贝)，sendfile只能从文件到socket
         ret = splice(STDIN_FILENO, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE|SPLICE_F_MOVE);
         ret = splice(pipefd[0], NULL, sockfd, NULL, 32768, SPLICE_F_MORE|SPLICE_F_MOVE);
      }
   }

   printf("client stop\n");
   close(sockfd);
   return 0;
}
