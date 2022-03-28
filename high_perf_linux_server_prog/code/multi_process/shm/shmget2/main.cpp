#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>

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

   pid_t pid = fork();
   if (pid == 0)
   {
      // child process
      shmid = shmget(key, 0, 0444);
      assert(shmid != -1);

      shmid_ds buf;
      int res = shmctl(shmid, IPC_STAT, &buf);
      assert(res == 0);
      const size_t shm_size = buf.shm_segsz;
      printf("The size of shard memory is %zu bytes\n", shm_size);

      addr = shmat(shmid, NULL, 0);
      assert(addr != reinterpret_cast<void *>(-1));
      char data[shm_size];
      memcpy(data, addr, shm_size);
      printf("Read from shared memory: \"%s\"\n", data);
      shmdt(addr);
      return 0;
   }

   assert(pid > 0);
   wait(NULL);

   // detache
   printf("shmdt in parent process\n");
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
