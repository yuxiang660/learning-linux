#include <pthread.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#define MAX 10

int buffer[MAX];
int fill = 0;
int use = 0;
int count = 0;

void put(int value) {
   buffer[fill] = value;
   fill = (fill + 1) % MAX;
   count++;
}

int get() {
   int tmp = buffer[use];
   use = (use + 1) % MAX;
   count--;
   return tmp;
}

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t fill_cond = PTHREAD_COND_INITIALIZER;

void* produce(void* arg) {
   int i;
   int loops = (intptr_t)arg;
   for (i = 0; i < loops; i++) {
      pthread_mutex_lock(&mutex);
      while (count == MAX)
         pthread_cond_wait(&empty_cond, &mutex);
      put(i);
      pthread_cond_signal(&fill_cond);
      pthread_mutex_unlock(&mutex);
   }
}

void* consume(void* arg) {
   int i;
   int loops = (intptr_t)arg;
   for (i = 0; i < loops; i++) {
      pthread_mutex_lock(&mutex);
      if (count == 0)
         pthread_cond_wait(&fill_cond, &mutex);
      int tmp = get();
      pthread_cond_signal(&empty_cond);
      pthread_mutex_unlock(&mutex);
      printf("%d\n", tmp);
   }
}

int main()
{
   pthread_t producer;
   pthread_t consumer1, consumer2;

   printf("main:begin\n");
   int rc;
   rc = pthread_create(&producer, NULL, produce, (void *)100); assert(rc == 0);
   rc = pthread_create(&consumer1, NULL, consume, (void *)49); assert(rc == 0);
   rc = pthread_create(&consumer2, NULL, consume, (void *)51); assert(rc == 0);

   rc = pthread_join(producer, NULL); assert(rc == 0);
   rc = pthread_join(consumer1, NULL); assert(rc == 0);
   rc = pthread_join(consumer2, NULL); assert(rc == 0);

   printf("main:end\n");

   return 0;
}
