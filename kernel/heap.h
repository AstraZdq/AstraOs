#pragma once

#include <stddef.h>
#include <stdint.h>

void heap_initialize();

void* kmalloc(size_t size);