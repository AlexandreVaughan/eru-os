#ifndef FILE_H
#define FILE_H


#include <stdint.h>
#include "types.h"
#include "pathparser.h"
#include "status.h"

typedef uint32_t file_seek_mode;
enum 
{
    SEEK_SET,
    SEEK_CUR,
    SEEK_END
};

typedef uint32_t file_mode;
enum
{
    FILE_MODE_READ,
    FILE_MODE_WRITE,
    FILE_MODE_APPEND,
    FILE_MODE_INVALID
};

struct disk;
typedef mem_ptr (*fs_open_func)(struct disk* disk, struct path_part* path, file_mode mode);
typedef int32_t (*fs_resolve_func)(struct disk* disk);

struct filesystem
{
    fs_resolve_func resolve;
    fs_open_func open;

    asciichar name[20];
};

struct file_descriptor
{
    int32_t index;
    struct filesystem* filesystem;

    // private data for internal file descriptor
    mem_ptr private_data;

    // disk that the file descriptor should be used on
    struct disk* disk;
};

void fs_init();
ret_code fopen(const asciistring filename, const asciistring mode);
void fs_insert_fileystem(struct filesystem* filesystem);
struct filesystem* fs_resolve(struct disk* disk);


#endif