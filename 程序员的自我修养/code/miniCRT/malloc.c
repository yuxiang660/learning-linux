#include "minicrt.h"

typedef struct _heap_header
{
   enum {
      HEAP_BLOCK_FREE = 0xABABABAB,
      HEAP_BLOCK_USED = 0xCDCDCDCD,
   } type;
   unsigned size; // Block size including header
   struct _heap_header* next;
   struct _heap_header* prev;
} heap_header;

#define ADDR_ADD(a,o) (((char*)(a)) + o)
#define HEADER_SIZE (sizeof(heap_header))

static heap_header* list_head = NULL;

void free(void* ptr)
{
   heap_header* header = (heap_header*)ADDR_ADD(ptr, -HEADER_SIZE);
   if (header->type != HEAP_BLOCK_USED) return;

   header->type = HEAP_BLOCK_FREE;
   if (header->prev != NULL && header->prev->type == HEAP_BLOCK_FREE) {
      // merge with previous block
      header->prev->next = header->next;
      if (header->next != NULL) header->next->prev = header->prev;
      header->prev->size += header->size;

      header = header->prev;
   }

   if (header->next != NULL && header->next->type == HEAP_BLOCK_FREE) {
      // merge with next block
      header->size += header->next->size;
      header->next = header->next->next;
   }
}

void* malloc(unsigned size)
{
   if(size == 0) return NULL;

   heap_header* header = list_head;
   while (header != NULL)
   {
      if (header->type == HEAP_BLOCK_USED) {
         header = header->next;
         continue;
      }

      if (header->size > size + HEADER_SIZE && header->size <= size + HEADER_SIZE * 2) {
         // The block has enough space but no need to split
         header->type = HEAP_BLOCK_USED;
         return ADDR_ADD(header, HEADER_SIZE);
      }

      if (header->size > size + HEADER_SIZE * 2) {
         // The block has additional space to split
         heap_header* next = (heap_header*)ADDR_ADD(header, size + HEADER_SIZE);
         next->prev = header;
         next->next = header->next;
         next->type = HEAP_BLOCK_FREE;
         next->size = header->size - (size + HEADER_SIZE);

         header->next = next;
         header->size = size + HEADER_SIZE;
         header->type = HEAP_BLOCK_USED;
         return ADDR_ADD(header, HEADER_SIZE);
      }
      header = header->next;
   }

   return NULL;
}

static void* brk(void* end_data_segment)
{
   void* ret = 0;

   // brk system call number is 45
   asm("movl $45,%%eax \n\t"
       "movl %1,%%ebx  \n\t"
       "int $0x80      \n\t"
       "movl %%eax, %0 \n\t": "=r"(ret) : "m"(end_data_segment));

   return ret;
}

int mini_crt_heap_init()
{
   void* base = NULL;
   heap_header* header = NULL;
   // 32MB heap size
   unsigned heap_size = 1024 * 1024 * 32;

   base = brk(0);
   void* end = ADDR_ADD(base, heap_size);
   end = brk(end);
   if(!end) return 0;

   header = (heap_header*)base;
   header->size = heap_size;
   header->type = HEAP_BLOCK_FREE;
   header->next = NULL;
   header->prev = NULL;

   list_head = header;
   return 1;
}
