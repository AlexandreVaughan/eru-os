#ifndef DISK_H
#define DISK_H

#include <stdint.h>
#include <stddef.h>
#include "types.h"
#include "status.h"

typedef uint32_t eruos_disk_type;

// real physical hard drive
#define ERUOS_DISK_TYPE_PHYSICAL_HDD 0

struct disk
{
    eruos_disk_type type;
    size_t sector_size;
};

void disk_search_and_init();
struct disk* disk_get(size_t index);
ret_code disk_read_block(struct disk* idisk, uint32_t lba, size_t total, mem_ptr buf);

#endif