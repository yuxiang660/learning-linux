#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t tid[2];
int ret1, ret2;

void *doSomeThing(void *arg)
{
   int thread_id = *((int*)(&arg));
   unsigned long i = 0;
   pthread_t id = pthread_self();

   sleep(2);

   if (pthread_equal(id, tid[0]))
   {
      printf("[Thread %d] First thread processing done\n", thread_id);
      ret1 = 100;
      pthread_exit(&ret1);
   }
   else
   {
      printf("[Thread %d] Second thread processing done\n", thread_id);
      ret2 = 200;
      pthread_exit(&ret2);
   }

   return NULL;
}

int main(void)
{
   int i = 0;
   int err;
   int *ptr[2];

   while (i < 2)
   {
      err = pthread_create(&(tid[i]), NULL, &doSomeThing, reinterpret_cast<void *>(i));
      if (err != 0)
         printf("can't create thread %d:[%s]", i, strerror(err));
      else
         printf("Thread %d created successfully\n", i);

      i++;
   }

   pthread_join(tid[0], (void **)&(ptr[0]));
   pthread_join(tid[1], (void **)&(ptr[1]));

   printf("\n return value from first thread is [%d]\n", *ptr[0]);
   printf("\n return value from second thread is [%d]\n", *ptr[1]);

   return 0;
}
