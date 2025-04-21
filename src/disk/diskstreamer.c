#include "diskstreamer.h"
#include "config.h"

#include "memory/heap/kheap.h"

struct disk_stream* diskstreamer_new(size_t disk_id)
{
    struct disk* disk = disk_get(disk_id);
    if (!disk)
    {
        return 0;
    }

    struct disk_stream *streamer = kzalloc(sizeof(struct disk_stream));
    streamer->pos = 0;
    streamer->disk = disk;
    return streamer;   
}

ret_code diskstreamer_seek(struct disk_stream* stream, disk_pos pos)
{
    stream->pos = pos;
    return 0;
}

ret_code diskstreamer_read(struct disk_stream* stream, mem_ptr out, size_t size)
{
    uint32_t sector = stream->pos / ERUOS_HDD_SECTOR_SIZE;
    uint32_t offset = stream->pos % ERUOS_HDD_SECTOR_SIZE;
    byte buf[ERUOS_HDD_SECTOR_SIZE];

    ret_code res = disk_read_block(stream->disk,sector,1,buf);
    if (res < 0)
    {
        return res;
    }

    // todo : extract to memcpy
    uint32_t size_to_read = size > ERUOS_HDD_SECTOR_SIZE ? ERUOS_HDD_SECTOR_SIZE : size;
    for (uint32_t idx = 0; idx < size_to_read; ++idx)
    {
        *(byte*)out++ = buf[offset+idx];
    }

    // todo : uses recursion, should be a while loop
    // risk of stack overflow
    stream->pos += size_to_read;
    if (size > ERUOS_HDD_SECTOR_SIZE)
    {
        res = diskstreamer_read(stream,out,size-ERUOS_HDD_SECTOR_SIZE);
    }

    return res;
}

void diskstreamer_close(struct disk_stream* stream)
{
    kfree(stream);
}