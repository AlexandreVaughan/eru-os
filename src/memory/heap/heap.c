#include "heap.h"
#include "kernel.h"
#include "memory/memory.h"
#include <stdbool.h>

static ret_code heap_validate_table(mem_ptr start, mem_ptr end, struct heap_table* table)
{
    size_t table_size  = (size_t) (end-start);
    size_t total_blocks = table_size / ERUOS_HEAP_BLOCK_SIZE;

    if (table->total != total_blocks)
    {
        return -EINVARG;
    }

    return ERUOS_ALL_OK;
}

static bool heap_validate_alignment(mem_ptr ptr)
{
    return ((uint32_t)ptr % ERUOS_HEAP_BLOCK_SIZE) == 0;
}

ret_code heap_create(struct heap *heap, mem_ptr mem_start, mem_ptr mem_end, struct heap_table *table)
{
    if (!heap_validate_alignment(mem_start) || !heap_validate_alignment(mem_end))
    {
        return -EINVARG;
    }  
    
    memset(heap, 0, sizeof(struct heap));
    heap->saddr = mem_start;
    heap->table = table;
    
    ret_code res = heap_validate_table(mem_start,mem_end,table);
    if (res < 0)
        return res;

    size_t table_size = sizeof(heap_block_table_entry) * table->total;
    memset(table->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, table_size);

    return ERUOS_ALL_OK;
}