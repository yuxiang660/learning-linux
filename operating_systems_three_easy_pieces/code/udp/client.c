// Client side implementation of UDP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080
#define MAXLINE 1024

void print_ipv4(const struct sockaddr *s)
{
   const struct sockaddr_in *sin = (const struct sockaddr_in *)s;
   char ip[INET_ADDRSTRLEN];
   uint16_t port;

   inet_ntop(AF_INET, &(sin->sin_addr), ip, sizeof(ip));
   port = htons(sin->sin_port);

   printf("host %s:%d\n", ip, port);
}

// Driver code
int main()
{
   int sockfd;
   char buffer[MAXLINE];
   char *hello = "Hello from client";
   struct sockaddr_in servaddr;

   // Creating socket file descriptor
   if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
   {
      perror("socket creation failed");
      exit(EXIT_FAILURE);
   }

   memset(&servaddr, 0, sizeof(servaddr));

   // Filling server information
   servaddr.sin_family = AF_INET;
   servaddr.sin_port = htons(PORT);
   servaddr.sin_addr.s_addr = INADDR_ANY;
   print_ipv4((const struct sockaddr *)&servaddr);

   int n, len;

   sendto(sockfd, (const char *)hello, strlen(hello),
          MSG_CONFIRM, (const struct sockaddr *)&servaddr,
          sizeof(servaddr));
   printf("Hello message sent.\n");
   print_ipv4((const struct sockaddr *)&servaddr);

   n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                MSG_WAITALL, (struct sockaddr *)&servaddr,
                &len);
   buffer[n] = '\0';
   printf("Server : %s\n", buffer);
   print_ipv4((const struct sockaddr *)&servaddr);

   close(sockfd);
   return 0;
}
