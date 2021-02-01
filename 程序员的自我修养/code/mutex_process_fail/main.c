#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

pthread_mutex_t lock;

void doSomeThing()
{
    printf("Press eny key to unlock the mutex\n");
    char c = getchar();
    printf("Character entered: ");
    putchar(c);
    printf("\n");
}

int main(void)
{
    int i = 0;
    int err;

    pthread_mutex_lock(&lock);
    doSomeThing();
    pthread_mutex_unlock(&lock);

    printf("End Process \n");

    return 0;
}