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
   printf("[Client] Test select\n");

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
   assert(sockfd >=0);

   int ret = connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address));
   assert(ret != -1);

   const char* oob_data = "abc";
   const char* normal_data = "123";
   send(sockfd, normal_data, strlen(normal_data), 0);
   send(sockfd, oob_data, strlen(oob_data), MSG_OOB);
   send(sockfd, normal_data, strlen(normal_data), 0);

   printf("Stop the client\n");
   close(sockfd);

   return 0;
}

