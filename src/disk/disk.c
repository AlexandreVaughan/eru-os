#include "disk.h"
#include "config.h"
#include "io/io.h"
#include "memory/memory.h"

struct disk primary_hdd;

ret_code disk_read_sector(uint32_t lba, size_t total, mem_ptr buf)
{
    outb(0x1f6, (lba >> 24) | 0xe0 );
    outb(0x1f2, total );
    outb(0x1f3, (ubyte)(lba & 0xff));
    outb(0x1f4, (ubyte)(lba >> 8));
    outb(0x1f5, (ubyte)(lba >> 16));
    outb(0x1f7, 0x20);

    uword *ptr  = (uword*)buf;
    for (size_t b = 0; b < total; ++b)
    {
        // wait for the buffer to be ready
        byte by = insb(0x1f7);
        while (!(by&0x08))
        {
            by = insb(0x1f7);
        }

        // copy from hdd to memory
        for (size_t i = 0; i < 256; ++i)
        {
            *ptr = insw(0x1f0);
            ptr++;
        }
    }
    return 0;
}

void disk_search_and_init()
{
    // for now, no real search, assume presence of a physical
    // primary hard drive and return it.
    memset(&primary_hdd,0,sizeof(primary_hdd));
    primary_hdd.type = ERUOS_DISK_TYPE_PHYSICAL_HDD;
    primary_hdd.sector_size = ERUOS_HDD_SECTOR_SIZE;
    primary_hdd.filesystem = fs_resolve(&primary_hdd);
}

struct disk* disk_get(size_t index)
{
    // for now only primary hdd is supported
    if (index != 0)
        return 0;
    return &primary_hdd;
}

ret_code disk_read_block(struct disk* idisk, uint32_t lba, size_t total, mem_ptr buf)
{
    if (idisk != &primary_hdd)
    {
        return -EIO;
    }

    return disk_read_sector(lba, total, buf);
}