#include "kheap.h"
#include "heap.h"
#include "config.h"
#include "kernel.h"
#include "memory/memory.h"

struct heap kernel_heap;
struct heap_table kernel_heap_table;

void kheap_init()
{
    size_t total_table_entries = ERUOS_HEAP_SIZE_BYTES / ERUOS_HEAP_BLOCK_SIZE;
    kernel_heap_table.entries = (heap_block_table_entry*)(ERUOS_HEAP_TABLE_ADDRESS);
    kernel_heap_table.total = total_table_entries;

    mem_ptr end = (mem_ptr)(ERUOS_HEAP_ADDRESS + ERUOS_HEAP_SIZE_BYTES);
    ret_code res = heap_create(&kernel_heap,(mem_ptr)(ERUOS_HEAP_ADDRESS),end,&kernel_heap_table);

    if (res<0)
    {
        print("Failed to create heap");
    }
}

mem_ptr kmalloc(size_t size)
{
    return heap_malloc(&kernel_heap,size);
}

mem_ptr kzalloc(size_t size)
{
    mem_ptr ptr = kmalloc(size);
    if (!ptr)
    {
        return ptr;
    }
    memset(ptr, 0, size);
    return ptr;

}

void kfree(mem_ptr ptr)
{
    return heap_free(&kernel_heap,ptr);
}