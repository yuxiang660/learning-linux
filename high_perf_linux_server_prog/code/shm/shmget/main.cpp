#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Refer to: https://users.cs.cf.ac.uk/Dave.Marshall/C/node27.html#SECTION002731000000000000000

#define SHMSZ 27
const char buf[] = "Hello World";

int main(void)
{
   key_t key;
   if ((key = ftok(".", 'a')) == -1)
   {
      perror("ftok");
      exit(1);
   }

   int shmid;
   // create a shared memory with SHMSZ size
   if ((shmid = shmget(key, SHMSZ, 0666 | IPC_CREAT)) == -1)
   {
      perror("shmget");
      exit(1);
   }

   // attach the segment to our data space.
   void *addr;
   if ((addr = shmat(shmid, NULL, 0)) == reinterpret_cast<void *>(-1))
   {
      perror("shmat");
      exit(1);
   }

   memcpy(addr, buf, sizeof(buf));

   // detache
   if (shmdt(addr) == -1)
   {
      perror("shmdt");
      exit(1);
   }

   // delete
   if (shmctl(shmid, IPC_RMID, NULL) == -1)
   {
      perror("shmctl");
      exit(1);
   }

   return 0;
}
