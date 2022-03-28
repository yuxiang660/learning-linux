#include "tpool.h"

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static void* thread_start(void* args)
{
   tpool_t *pool = (tpool_t*)args;

   while (1)
   {
      pthread_mutex_lock(&pool->work_queue_lock);
      while (!pool->work_queue_head && !pool->shutdown)
      {
         pthread_cond_wait(&pool->work_queue_ready, &pool->work_queue_lock);
      }

      if (pool->shutdown)
      {
         pthread_mutex_unlock(&pool->work_queue_lock);
         pthread_exit(NULL);
      }

      work_queue_t* work = pool->work_queue_head;
      pool->work_queue_head = pool->work_queue_head->next;
      pthread_mutex_unlock(&pool->work_queue_lock);

      // Run task
      work->work_routine(work->args);
      free(work);
   }

   return NULL;
}

int create_tpool(tpool_t** pool, size_t max_thread_num)
{
   *pool = (tpool_t*)malloc(sizeof(tpool_t));
   if (NULL == *pool)
   {
      printf("in %s, malloc tpool_t failed, errno = %d, explain: %s\n", __func__, errno, strerror(errno));
      exit(-1);
   }

   (*pool)->shutdown = false;
   (*pool)->maxnum_thread = max_thread_num;
   (*pool)->thread_arr = (pthread_t*)malloc(sizeof(pthread_t) * max_thread_num);
   if (NULL == (*pool)->thread_arr)
   {
      printf("in %s, init thread id failed, errno = %d, explain: %s\n", __func__, errno, strerror(errno));
      exit(-1);
   }

   (*pool)->work_queue_head = NULL;

   if (pthread_mutex_init(&((*pool)->work_queue_lock), NULL) != 0)
   {
      printf("in %s, init mutex failed, errno = %d, explain: %s\n", __func__, errno, strerror(errno));
      exit(-1);
   }

   if (pthread_cond_init(&((*pool)->work_queue_ready), NULL) != 0)
   {
      printf("in %s, init condition variable failed, errno = %d, explain: %s\n", __func__, errno, strerror(errno));
      exit(-1);
   }

   for (int i = 0; i < max_thread_num; i++)
   {
      if (pthread_create(&((*pool)->thread_arr[i]), NULL, thread_start, (void*)(*pool)) != 0)
      {
         printf("pthread_create failed\n");
         exit(-1);
      }
   }

   return 0;
}

void destroy_tpool(tpool_t* pool)
{
   if (pool->shutdown)
      return;

   pool->shutdown = true;
   pthread_mutex_lock(&pool->work_queue_lock);
   pthread_cond_broadcast(&pool->work_queue_ready);
   pthread_mutex_unlock(&pool->work_queue_lock);

   for (int i = 0; i < pool->maxnum_thread; i++)
   {
      pthread_join(pool->thread_arr[i], NULL);
   }
   free(pool->thread_arr);

   while (pool->work_queue_head)
   {
      work_queue_t *tmp_work = pool->work_queue_head;
      pool->work_queue_head = pool->work_queue_head->next;
      free(tmp_work);
   }

   pthread_mutex_destroy(&pool->work_queue_lock);
   pthread_cond_destroy(&pool->work_queue_ready);
   free(pool);
}

int add_task_2_tpool(tpool_t* pool, void*(*routine)(void*), void* args)
{
   if (!routine)
   {
      printf("routine is null\n");
      exit(-1);
   }

   work_queue_t *new_work = (work_queue_t*)malloc(sizeof(work_queue_t));
   if (NULL == new_work)
   {
      printf("in %s, malloc work failed, errno = %d, explain: %s\n", __func__, errno, strerror(errno));
      exit(-1);
   }

   new_work->work_routine = routine;
   new_work->args = args;
   new_work->next = NULL;

   pthread_mutex_lock(&pool->work_queue_lock);
   work_queue_t *last_work = pool->work_queue_head;
   if (NULL == last_work)
   {
      pool->work_queue_head = new_work;
   }
   else
   {
      while (last_work->next)
      {
         last_work = last_work->next;
      }
      last_work->next = new_work;
   }

   pthread_cond_signal(&pool->work_queue_ready);
   pthread_mutex_unlock(&pool->work_queue_lock);

   return 0;
}

