#include "heap.h"

#define HEAP_START 0x01000000
#define HEAP_SIZE  0x00100000

static uint8_t * heap_current = (uint8_t *)HEAP_START;

void heap_initialize()
{
    heap_current = (uint8_t *)HEAP_START;
}

void* kmalloc(size_t size)
{
   if ((uint32_t)(heap_current + size) >=
       (HEAP_START + HEAP_SIZE))
   {
       return 0; 
   }

   void* allocation = heap_current;

   heap_current += size;

   return allocation;
}