#include <stdio.h>
#include <libgen.h> // basename
#include <stdlib.h> // atoi
#include <assert.h>
#include <string.h> // bzero
#include <unistd.h> // sleep, close
#include <errno.h>
#include <poll.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define USER_LIMIT 5 // 最大用户数量
#define BUFFER_SIZE 64
#define FD_LIMIT 65535

struct client_data
{
   sockaddr_in address;
   char* write_buf;
   char buf[BUFFER_SIZE];
};

int setnonblocking(int fd)
{
   int old_option = fcntl(fd, F_GETFL);
   int new_option = old_option | O_NONBLOCK;
   fcntl(fd, F_SETFL, new_option);
   return old_option;
}

int main(int argc, char* argv[])
{
   printf("[Server] Test demo - chat\n");

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
   int reuse = 1;
   int ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
   assert(ret != -1);

   ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
   assert(ret != -1);

   ret = listen(listenfd, 5);
   assert(ret != -1);

   client_data *users = new client_data[FD_LIMIT];
   // 尽管我们分配了足够多的client_data对象，但是为了提高poll的性能，仍然有必要限制用户的数量
   pollfd fds[USER_LIMIT + 1];
   int user_counter = 0;
   for (int i = 1; i <= USER_LIMIT; i++)
   {
      fds[i].fd = -1;
      fds[i].events = 0;
   }
   fds[0].fd = listenfd;
   fds[0].events = POLLIN | POLLERR;
   fds[0].revents = 0;

   while (1)
   {
      ret = poll(fds, user_counter+1, -1);
      if (ret < 0)
      {
         printf("poll failure\n");
         break;
      }

      for (int i = 0; i < user_counter+1; i++)
      {
         if ((fds[i].fd == listenfd) && (fds[i].revents & POLLIN))
         {
            struct sockaddr_in client_address;
            socklen_t client_addrlength = sizeof(client_address);
            int connfd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlength);
            if (connfd < 0)
            {
               printf("errno is: %d\n", errno);
               continue;
            }
            // 如果请求太多，则关闭新到的连接
            if (user_counter >= USER_LIMIT)
            {
               const char* info = "too many users\n";
               printf("%s", info);
               send(connfd, info, strlen(info), 0);
               close(connfd);
               continue;
            }
            // 对于新的连接，同时修改fds和users
            user_counter++;
            users[connfd].address = client_address;
            setnonblocking(connfd);
            fds[user_counter].fd = connfd;
            fds[user_counter].events = POLLIN | POLLRDHUP | POLLERR;
            fds[user_counter].revents = 0;
            printf("comes a new user, now have %d users, connection %d\n", user_counter, connfd);
         }
         else if (fds[i].revents & POLLERR)
         {
            printf("get an error from %d\n", fds[i].fd);
            int error = 0;
            socklen_t length = sizeof(error);
            if (getsockopt(fds[i].fd, SOL_SOCKET, SO_ERROR, &error, &length) < 0)
            {
               printf("get socket option failed\n");
            }

            printf("socket hash the error: %d, close the socket: %d\n", error, fds[i].fd);
            continue;
         }
         else if (fds[i].revents & POLLRDHUP)
         {
            // 如果客户端关闭连接，则服务器也关闭对应的连接
            users[fds[i].fd] = users[fds[user_counter].fd];
            close(fds[i].fd);
            fds[i] = fds[user_counter];
            i--;
            user_counter--;
            printf("a client left\n");
         }
         else if (fds[i].revents & POLLIN)
         {
            int connfd = fds[i].fd;
            memset(users[connfd].buf, '\0', BUFFER_SIZE);
            ret = recv(connfd, users[connfd].buf, BUFFER_SIZE-1, 0);
            printf("get %d bytes of client data %s from %d\n", ret, users[connfd].buf, connfd);
            if (ret < 0)
            {
               if (errno != EAGAIN)
               {
                  close(connfd);
                  users[fds[i].fd] = users[fds[user_counter].fd];
                  fds[i] = fds[user_counter];
                  i--;
                  user_counter--;
               }
            }
            else if (ret == 0)
            {
               printf("client is closed\n");
            }
            else
            {
               for (int j = 1; j <= user_counter; ++j)
               {
                  if (fds[j].fd == connfd)
                  {
                     continue;
                  }
                  fds[j].events &= ~POLLIN;
                  fds[j].events |= POLLOUT;
                  users[fds[j].fd].write_buf = users[connfd].buf;
               }
            }
         }
         else if (fds[i].revents & POLLOUT)
         {
            int connfd = fds[i].fd;
            if (!users[connfd].write_buf)
            {
               printf("the write_buf is NULL on connection: %d\n", connfd);
               fds[i].events &= ~POLLOUT;
               fds[i].events |= POLLIN;
               continue;
            }
            ret = send(connfd, users[connfd].write_buf, strlen(users[connfd].write_buf), 0);
            users[connfd].write_buf = NULL;
            fds[i].events &= ~POLLOUT;
            fds[i].events |= POLLIN;
         }
      }
   }

   printf("Stop the server\n");
   delete [] users;
   close(listenfd);

   return 0;
}

