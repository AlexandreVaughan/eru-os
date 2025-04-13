#include "disk.h"
#include "io/io.h"

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

