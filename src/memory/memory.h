#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include "types.h"
#include <stdint.h>


mem_ptr memset(mem_ptr ptr, mem_byte_value val, size_t size);
int32_t memcmp(mem_ptr s1, mem_ptr s2, size_t count);

#endif