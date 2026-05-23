#pragma once

#include <stdint.h>

void paging_initialize();
void paging_map_device(uint32_t physical_addr, uint32_t size);
