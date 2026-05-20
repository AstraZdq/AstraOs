#include "heap.h"

#include <stdint.h>

#define HEAP_START 0x00200000
#define HEAP_SIZE  0x00100000

typedef struct heap_block
{
    size_t size;

    int free;

    struct heap_block* next;

} heap_block_t;

static heap_block_t* heap_head = (heap_block_t*)HEAP_START;

void heap_initialize()
{
    heap_head->size =
        HEAP_SIZE - sizeof(heap_block_t);

    heap_head->free = 1;

    heap_head->next = 0;
}

static void split_block(
    heap_block_t* block,
    size_t size)
{
    heap_block_t* new_block =
        (heap_block_t*)
        ((uint8_t*)block +
        sizeof(heap_block_t) +
        size);

    new_block->size =
        block->size -
        size -
        sizeof(heap_block_t);

    new_block->free = 1;

    new_block->next = block->next;

    block->size = size;

    block->free = 0;

    block->next = new_block;
}

void* kmalloc(size_t size)
{
    heap_block_t* current = heap_head;

    while (current)
    {
        if (current->free &&
            current->size >= size)
        {
            if (current->size >
                size + sizeof(heap_block_t))
            {
                split_block(current, size);
            }

            current->free = 0;

            return
                (uint8_t*)current +
                sizeof(heap_block_t);
        }

        current = current->next;
    }

    return 0;
}

void kfree(void* ptr)
{
    if (!ptr)
    {
        return;
    }

    heap_block_t* block =
        (heap_block_t*)
        ((uint8_t*)ptr -
        sizeof(heap_block_t));

    block->free = 1;
}