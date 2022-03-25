#include <sys/signal.h>
#include <stdio.h>
#include <event.h>

void signal_cb(int fd, short event, void* argc)
{
   // argc is passed in when create the signal event handler
   struct event_base* base = reinterpret_cast<event_base*>(argc);
   struct timeval delay = {2, 0};
   printf("Caugth an interrupt signal; exiting cleanly in two seconds...\n");
   event_base_loopexit(base, &delay);
}

void timeout_cb(int fd, short event, void* argc)
{
   printf("timeout\n");
}

int main()
{
   // 1. Create a Reactor object
   struct event_base* base = event_init();

   // 2.1 Create a signal event handler, binding with signal number SIGINT
   struct event* signal_event = evsignal_new(base, SIGINT, signal_cb, base);
   // 2.2 Register the handler to event demultiplexer
   event_add(signal_event, NULL);

   // 3.1 Create a timer event handler, binding with timer fd(-1) internally
   timeval tv = {1, 0};
   struct event* timeout_event = evtimer_new(base, timeout_cb, NULL);
   // 3.2 Register the handler to event demultiplexer
   event_add(timeout_event, &tv);

   // 4 Start event loop
   event_base_dispatch(base);

   // 5 Free resources
   event_free(timeout_event);
   event_free(signal_event);
   event_base_free(base);
}
