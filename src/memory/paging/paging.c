 #include "paging.h"
 #include "memory/heap/kheap.h"
 #include "status.h"


void paging_load_directory(uint32_t *directory);

static uint32_t* current_directory = 0;

struct paging_4gb_chunk* paging_new_4gb(uint8_t flags)
{
    uint32_t *directory = kzalloc(sizeof(uint32_t)*PAGING_TOTAL_ENTRIES_PER_TABLE);
    uint32_t offset=0;
    for (uint32_t i = 0; i < PAGING_TOTAL_ENTRIES_PER_TABLE; ++i)
    {
        uint32_t* entry = kzalloc(sizeof(uint32_t) * PAGING_TOTAL_ENTRIES_PER_TABLE);
        for (uint32_t j = 0; j < PAGING_TOTAL_ENTRIES_PER_TABLE; ++j)
        {
            entry[j] = (offset + j*PAGING_PAGE_SIZE) | flags;
        }
        offset += PAGING_TOTAL_ENTRIES_PER_TABLE*PAGING_PAGE_SIZE;
        // this works because we're aligned to 4Kb
        directory[i] = (uint32_t)entry | flags | PAGING_IS_WRITABLE;
    }

    struct paging_4gb_chunk* chunk_4gb = kzalloc(sizeof(struct paging_4gb_chunk));
    chunk_4gb->directory_entry = directory;
    return chunk_4gb;
}

void paging_switch(uint32_t *directory)
{
    paging_load_directory(directory);
    current_directory = directory;     
}

uint32_t* paging_4gb_chunk_get_directory(struct paging_4gb_chunk *chunk)
{
    return chunk->directory_entry;
}

bool paging_is_aligned(mem_ptr address)
{
    return ((uint32_t)address % PAGING_PAGE_SIZE) == 0;
}

ret_code paging_get_indexes(mem_ptr virtual_address, uint32_t *directory_index_out, uint32_t *table_index_out)
{
    if (!paging_is_aligned(virtual_address))
    {
        return -EINVARG;
    }

    *directory_index_out = ((uint32_t)virtual_address / (PAGING_TOTAL_ENTRIES_PER_TABLE*PAGING_PAGE_SIZE));
    *table_index_out = ((uint32_t)virtual_address % (PAGING_TOTAL_ENTRIES_PER_TABLE*PAGING_PAGE_SIZE) / PAGING_PAGE_SIZE);
    return 0;

}

ret_code paging_set(uint32_t *directory, mem_ptr virtual_address, uint32_t physical_address)
{
    if (!paging_is_aligned(virtual_address))
    {
        return -EINVARG;
    }

    uint32_t dir_index = 0;
    uint32_t table_index = 0;

    ret_code res = paging_get_indexes(virtual_address, &dir_index, &table_index);
    if (res < 0)
    {
        return res;
    }

    uint32_t entry = directory[dir_index];
    uint32_t *table = (uint32_t*)(entry & 0xfffff000); // this works because we're aligned to 4Kb
    table[table_index]=physical_address;
    return 0;
}