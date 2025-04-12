#ifndef PAGING_H
#define PAGING_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "status.h"
#include "types.h"

#define PAGING_CACHE_DISABLED   0b00010000
#define PAGING_WRITE_THROUGH    0b00001000
#define PAGING_ACCESS_FROM_ALL  0b00000100
#define PAGING_IS_WRITABLE      0b00000010
#define PAGING_IS_PRESENT       0b00000001


#define PAGING_TOTAL_ENTRIES_PER_TABLE  1024
#define PAGING_PAGE_SIZE  4096

struct paging_4gb_chunk
{
    uint32_t* directory_entry;
};

void paging_switch(uint32_t *directory);
struct paging_4gb_chunk* paging_new_4gb(uint8_t flags);
void enable_paging();

uint32_t* paging_4gb_chunk_get_directory(struct paging_4gb_chunk *chunk);

bool paging_is_aligned(mem_ptr address);
ret_code paging_get_indexes(mem_ptr virtual_address, uint32_t *directory_index_out, uint32_t *table_index_out);
ret_code paging_set(uint32_t *directory, mem_ptr virtual_address, uint32_t physical_address);

#endif