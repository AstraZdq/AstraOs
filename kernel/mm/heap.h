#pragma once

#include <stddef.h>

void heap_initialize();

void* kmalloc(size_t size);

void kfree(void* ptr);
