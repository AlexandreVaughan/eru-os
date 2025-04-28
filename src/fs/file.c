#include "file.h"
#include "config.h"
#include "memory/memory.h"
#include "memory/heap/kheap.h"

struct filesystem* filesystems[ERUOS_MAX_FILESYSTEMS];
struct file_descriptor* file_descriptors[ERUOS_MAX_FILEDESCRIPTORS];


static struct filesystem** fs_get_free_fileystem()
{
    for (int32_t i = 0; i < ERUOS_MAX_FILESYSTEMS; ++i)
    {
        if (filesystems[i] == 0)
        {
            return &(filesystems[i]);
        }
    }

    return 0;
}

void fs_insert_fileystem(struct filesystem* filesystem)
{
    struct filesystem **fs;
    fs = fs_get_free_fileystem();
    if (!fs)
    {
        // panic : problem inserting filesystem
        while(1);
    }
    *fs = filesystem;
}

static void fs_static_load()
{
    // fs_insert_fileystem(fat16_init())
}

void fs_load()
{
    memset(filesystems,0,sizeof(filesystems));
    fs_static_load();
}

void fs_init()
{
    memset(file_descriptors,0,sizeof(file_descriptors));
    fs_load();
}

static ret_code file_new_descriptor(struct file_descriptor** desc_out)
{
    ret_code res = -ENOMEM;
    for (int32_t i = 0; i < ERUOS_MAX_FILEDESCRIPTORS; ++i)
    {
        if (file_descriptors[i] == 0)
        {
            struct file_descriptor* desc = kzalloc(sizeof(struct file_descriptor));
            desc->index = i+1;
            file_descriptors[i] = desc;
            *desc_out = desc;
            res = 0;
            break;
        }
    }

    return res;
}

static struct file_descriptor* file_get_descriptor(int32_t fd)
{
    if (fd <= 0 || fd >= ERUOS_MAX_FILEDESCRIPTORS)
    {
        return 0;
    }

    int32_t idx = fd-1;
    return file_descriptors[idx];
}

struct filesystem* fs_resolve(struct disk *disk)
{
    for (int32_t i = 0; i < ERUOS_MAX_FILEDESCRIPTORS; ++i)
    {
        if (filesystems[i] == 0 || filesystems[i]->resolve(disk) != 0)
        {
            continue;
        }
        return filesystems[i];        
    }
    return 0;

}

ret_code fopen(const asciistring filename, const asciistring mode)
{
    return -EIO;
}