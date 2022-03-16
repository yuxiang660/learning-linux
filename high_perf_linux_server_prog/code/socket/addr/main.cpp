#include <arpa/inet.h>
#include <stdio.h>

int main()
{
   printf("Test socket addr\n");

   struct in_addr addr1;
   inet_aton("1.2.3.4", &addr1);
   char *szValue1 = inet_ntoa(addr1);

   struct in_addr addr2;
   inet_aton("10.194.71.60", &addr2);
   char *szValue2 = inet_ntoa(addr2);

   printf("address 1: %s\n", szValue1);
   printf("address 2: %s\n", szValue2);

   return 0;
}
