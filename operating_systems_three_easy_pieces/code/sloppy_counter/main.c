#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define NUMBCPUS 4

typedef struct counter_t {
   int global;
   pthread_mutex_t glock;
   int local[NUMBCPUS];
   pthread_mutex_t llock[NUMBCPUS];
   int threshold;
} counter_t;

void init(counter_t* c, int threshold) {
   c->threshold = threshold;
   c->global = 0;
   for(int i = 0; i < NUMBCPUS; i++) {
      c->local[i] = 0;
      pthread_mutex_init(&c->llock[i], NULL);
   }
}

// update: usually, just grab local lock and update local amount once local count has risen by threshold,
// grab global lock and transfer local values to it
void update(counter_t* c, int threadID, int amt) {
   pthread_mutex_lock(&c->llock[threadID]);
   c->local[threadID] += amt;
   if (c->local[threadID] >= c->threshold) {
      pthread_mutex_lock(&c->glock);
      c->global += c->local[threadID];
      pthread_mutex_unlock(&c->glock);
      c->local[threadID] = 0;
   }
   pthread_mutex_unlock(&c->llock[threadID]);
}

int get(counter_t *c) {
   pthread_mutex_lock(&c->glock);
   int val = c->global;
   pthread_mutex_unlock(&c->glock);
   return val;
}

counter_t counter;

void* mythread(void* arg) {
   int m = (intptr_t)arg;
   printf("ThreadID is %d\n", m);
   for (int i = 0; i < 10000000; i++) {
      update(&counter, m, 1);
   }
   long long rc = get(&counter);
   return (void*)rc;
}

int main()
{
   printf("main:begin\n");

   pthread_t p0, p1, p2, p3, p4, p5;
   int r0, r1, r2, r3, r4, r5;
   init(&counter, 100);

   int rc;

   rc = pthread_create(&p0, NULL, mythread, (void *)0); assert(rc == 0);
   rc = pthread_create(&p1, NULL, mythread, (void *)1); assert(rc == 0);
   rc = pthread_create(&p2, NULL, mythread, (void *)2); assert(rc == 0);
   rc = pthread_create(&p3, NULL, mythread, (void *)3); assert(rc == 0);
   rc = pthread_create(&p4, NULL, mythread, (void *)2); assert(rc == 0);
   rc = pthread_create(&p5, NULL, mythread, (void *)3); assert(rc == 0);

   rc = pthread_join(p0, (void **)&r0); assert(rc == 0);
   rc = pthread_join(p1, (void **)&r1); assert(rc == 0);
   rc = pthread_join(p2, (void **)&r2); assert(rc == 0);
   rc = pthread_join(p3, (void **)&r3); assert(rc == 0);
   rc = pthread_join(p4, (void **)&r4); assert(rc == 0);
   rc = pthread_join(p5, (void **)&r5); assert(rc == 0);

   printf("thread0 returns %d\n", r0);
   printf("thread1 returns %d\n", r1);
   printf("thread2 returns %d\n", r2);
   printf("thread3 returns %d\n", r3);
   printf("thread4 returns %d\n", r4);
   printf("thread5 returns %d\n", r5);

   int ret = get(&counter);
   printf("main returns %d\n", ret);

   printf("main:end\n");

   return 0;
}
