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

static uint32_t heap_block_count_for_size(uint32_t val)
{
    uint32_t remainder = val % ERUOS_HEAP_BLOCK_SIZE;
    return (val / ERUOS_HEAP_BLOCK_SIZE + (remainder > 0 ? 1 : 0)); 
}

static heap_entry_type heap_get_entry_type(heap_block_table_entry entry)
{
    return entry&0x0f;
}

uint32_t heap_get_start_block(struct heap * heap, uint32_t block_count)
{
    struct heap_table *table = heap->table;
    size_t total_heap_blocks = table->total;
    uint32_t block_start = -1;
    uint32_t current_free_block_count = 0;

    for (uint32_t idx = 0; idx < total_heap_blocks; ++idx)
    {
        heap_entry_type entry_type = heap_get_entry_type(table->entries[idx]);
        if (entry_type != HEAP_BLOCK_TABLE_ENTRY_FREE)
        {
            current_free_block_count = 0;
            block_start = -1;
            continue;
        }

        if (block_start == -1)
        {
            block_start = idx;
        }
        current_free_block_count ++;

        if (current_free_block_count == block_count)
        {
            break;
        } 
    }

    if (block_start == -1)
    {
        return -ENOMEM;
    }
    return block_start;
}

mem_ptr heap_block_to_address(struct heap * heap, uint32_t start_block_index)
{
    return heap->saddr + start_block_index*ERUOS_HEAP_BLOCK_SIZE;
}

uint32_t heap_address_to_block(struct heap * heap, mem_ptr address)
{
    return ((uint32_t)(address - heap->saddr)) / ERUOS_HEAP_BLOCK_SIZE;
}

void heap_mark_blocks_taken(struct heap * heap,uint32_t start_block_index,uint32_t block_count)
{
    struct heap_table *table = heap->table;

    heap_block_table_entry entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;
    if (block_count > 1)
    {
        entry |= HEAP_BLOCK_HAS_NEXT;
    }

    uint32_t end_block_index = start_block_index+block_count-1;
    for (uint32_t idx = start_block_index; idx <= end_block_index; ++idx)
    {
        table->entries[idx] = entry;
        entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
        if (idx != end_block_index-1)
        {
            entry |= HEAP_BLOCK_HAS_NEXT; 
        }
    }
}

void heap_mark_blocks_free(struct heap * heap,uint32_t start_block_index)
{
    struct heap_table *table = heap->table;

    for (uint32_t idx = start_block_index; idx <= table->total; ++idx)
    {
        heap_block_table_entry entry = table->entries[idx];
        table->entries[idx] = HEAP_BLOCK_TABLE_ENTRY_FREE;
        if (!(entry&HEAP_BLOCK_HAS_NEXT))
        {
            break;
        }
    }
}


mem_ptr heap_malloc_blocks(struct heap * heap, uint32_t block_count)
{
    mem_ptr address = 0;
    size_t start_block = heap_get_start_block(heap, block_count);
    if (start_block < 0)
    {
        return address;
    }
    address = heap_block_to_address(heap, start_block);
    heap_mark_blocks_taken(heap,start_block,block_count);
    return address;
}


mem_ptr heap_malloc(struct heap * heap, size_t size)
{
    uint32_t total_blocks = heap_block_count_for_size(size);
    return heap_malloc_blocks(heap, total_blocks);
}

void heap_free(struct heap * heap, mem_ptr ptr)
{
    heap_mark_blocks_free(heap, heap_address_to_block(heap,ptr));
}