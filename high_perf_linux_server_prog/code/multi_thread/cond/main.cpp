#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

// Declaration of thread condition variable
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;

// declaring mutex
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int done = 1;

// Thread function
void *foo(void* arg)
{
   // acquire a lock
   pthread_mutex_lock(&lock);
   if (done == 1)
   {
      // let's wait on condition variable cond1
      done = 2;
      printf("[Thread %ld] Waiting on condition variable cond1\n", pthread_self());
      // lock will be released when waiting, and lock again when continue
      pthread_cond_wait(&cond1, &lock);
   }
   else
   {
      // Let's signal condition variable cond1
      printf("[Thread %ld] Signaling condition variable cond1\n", pthread_self());
      pthread_cond_signal(&cond1);
   }

   // release lock
   pthread_mutex_unlock(&lock);

   printf("[Thread %ld] Returning thread\n", pthread_self());

   return NULL;
}

// Driver code
int main()
{
   pthread_t tid1, tid2;

   // Create thread 1
   pthread_create(&tid1, NULL, foo, NULL);

   // sleep for 1 sec so that thread 1
   // would get a chance to run first
   sleep(1);

   // Create thread 2
   pthread_create(&tid2, NULL, foo, NULL);

   // wait for the completion of thread 2
   pthread_join(tid1, NULL);
   pthread_join(tid2, NULL);

   return 0;
}
