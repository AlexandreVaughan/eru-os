#ifndef KHEAP_H
#define KHEAP_H

#include <stdint.h>
#include <stddef.h>
#include "types.h"

void kheap_init();
mem_ptr kmalloc(size_t size);
void kfree(mem_ptr ptr);

#endif