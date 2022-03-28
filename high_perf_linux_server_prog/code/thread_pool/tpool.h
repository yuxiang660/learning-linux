#pragma once

#include <pthread.h>
#include <ctype.h>

struct work_queue_t
{
   void* (*work_routine)(void*);
   void* args;
   struct work_queue_t* next;
};

struct tpool_t
{
   bool shutdown;
   size_t maxnum_thread;
   pthread_t *thread_arr; // thread array
   work_queue_t *work_queue_head;
   pthread_cond_t work_queue_ready;
   pthread_mutex_t work_queue_lock;
};


int create_tpool(tpool_t** pool, size_t max_thread_num);
void destroy_tpool(tpool_t* pool);
int add_task_2_tpool(tpool_t* pool, void*(*routine)(void*), void* args);
