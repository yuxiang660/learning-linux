#include <pthread.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>

void* mythread(void* arg) {
   int m = (intptr_t)arg;
   printf("%d\n", m);

   return (void*)(arg+1);
}

int main(int argc, char* argv[]) {
   pthread_t p;
   int rc, m;

   printf("main:begin\n");
   rc = pthread_create(&p, NULL, mythread, (void *)100); assert(rc == 0);
   rc = pthread_join(p, (void **)&m); assert(rc == 0);
   printf("returned %d\n", m);
   printf("main:end\n");
   return 0;
}
