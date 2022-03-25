#pragma once

#include <time.h>
#include <netinet/in.h>
#include <stdio.h>

#define BUFFER_SIZE 64

class util_timer;

struct client_data
{
   sockaddr_in address;
   int sockfd;
   char buf[BUFFER_SIZE];
   util_timer* timer;
};

class util_timer
{
public:
   util_timer() : prev(NULL), next(NULL) {}
public:
   time_t expire; // 任务的超时时间，这里使用绝对时间
   void (*cb_func)(client_data*); //任务回调函数
   // 回调函数处理的客户数据，由定时器的执行者传递给回调函数
   client_data* user_data;
   util_timer* prev;
   util_timer* next;
};

class sort_timer_lst
{
public:
   sort_timer_lst() : head(NULL), tail(NULL) {}
   ~sort_timer_lst()
   {
      util_timer* tmp = head;
      while (tmp)
      {
         head = tmp->next;
         delete tmp;
         tmp = head;
      }
   }

   // 将目标定时器timer添加到链表中
   void add_timer(util_timer* timer)
   {
      if (!timer)
         return;
      if (!head)
      {
         head = tail = timer;
         return;
      }
      if (timer->expire < head->expire)
      {
         timer->next = head;
         head->prev = timer;
         head = timer;
         return;
      }
      add_timer(timer, head);
   }

   void adjust_timer(util_timer* timer)
   {
      if (!timer)
         return;
      util_timer* tmp = timer->next;
      if (!tmp || timer->expire < tmp->expire)
      {
         return;
      }
      if (timer == head)
      {
         head = head->next;
         head->prev = NULL;
         timer->next = NULL;
         add_timer(timer, head);
      }
      else
      {
         timer->prev->next = timer->next;
         timer->next->prev = timer->prev;
         add_timer(timer, timer->next);
      }
   }

   void del_timer(util_timer* timer)
   {
      if (!timer)
         return;
      if (timer == head && timer == tail)
      {
         delete timer;
         head = NULL;
         tail = NULL;
         return;
      }
      if (timer == head)
      {
         head = head->next;
         head->prev = NULL;
         delete timer;
         return;
      }
      if (timer == tail)
      {
         tail = tail->prev;
         tail->next = NULL;
         delete timer;
         return;
      }
      timer->prev->next = timer->next;
      timer->next->prev = timer->prev;
      delete timer;
   }

   // SIGALRM信号每次被触发就在其信号处理函数中执行一次tick函数，以处理链表上到期的任务
   void tick()
   {
      if (!head)
      {
         return;
      }
      printf("timer tick\n");
      time_t cur = time(NULL); // 当前系统时间
      util_timer* tmp = head;
      while (tmp)
      {
         if (cur < tmp->expire)
         {
            break;
         }
         // 调用定时器的回调函数，以执行定时任务
         tmp->cb_func(tmp->user_data);
         head = tmp->next;
         if (head)
         {
            head->prev = NULL;
         }
         delete tmp;
         tmp = head;
      }
   }
private:
   void add_timer(util_timer* timer, util_timer* header)
   {
      util_timer* tmp = head;
      // find tmp node, tmp < timer <= tmp->next
      while (tmp != tail && tmp->expire < timer->expire && tmp->next->expire >= timer->expire)
      {
         tmp = tmp->next;
      }
      // insert timer right after tmp
      timer->next = tmp->next;
      timer->prev = tmp;
      tmp->next = timer;
      timer->next->prev = timer;
   }

private:
   util_timer* head;
   util_timer* tail;
};
