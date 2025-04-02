#ifndef HEAP_H
#define HEAP_H

#include "config.h"
#include <stdint.h>
#include <stddef.h>
#include "types.h"

#define HEAP_BLOCK_TABLE_ENTRY_TAKEN 0x01
#define HEAP_BLOCK_TABLE_ENTRY_FREE  0x00

#define HEAP_BLOCK_HAS_NEXT  0b10000000
#define HEAP_BLOCK_IS_FREE   0b01000000

typedef ubyte heap_block_table_entry;


struct heap_table 
{
    heap_block_table_entry* entries;
    size_t total;
};

struct heap
{
    struct heap_table* table;
    mem_ptr saddr;
};

ret_code heap_create(struct heap *heap, mem_ptr mem_start, mem_ptr mem_end, struct heap_table *table);

#endif
