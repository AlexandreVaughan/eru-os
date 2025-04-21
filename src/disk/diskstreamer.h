#ifndef DISKSTREAMER_H
#define DISKSTREAMER_H

#include "disk.h"

typedef int32_t disk_pos;

struct disk_stream
{
    disk_pos pos;
    struct disk* disk;
};

struct disk_stream* diskstreamer_new(size_t disk_id);
ret_code diskstreamer_seek(struct disk_stream* stream, disk_pos pos);
ret_code diskstreamer_read(struct disk_stream* stream, mem_ptr out, size_t size);
void diskstreamer_close(struct disk_stream* stream);

#endif
