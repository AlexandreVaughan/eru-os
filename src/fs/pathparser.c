#include "pathparser.h"

#include "status.h"
#include "config.h"
#include <stddef.h>
#include "string/string.h"
#include "memory/heap/kheap.h"
#include "memory/memory.h"

static bool pathparser_valid_format(const asciistring path)
{
    size_t len = strnlen(path,ERUOS_MAX_PATH);
    return len >= 3 && 
           is_digit(path[0]) && 
           memcmp((mem_ptr)&path[1],":/", 2) == 0;
}

static int32_t pathparser_get_drive(asciistring* path)
{
    if (!pathparser_valid_format(*path))
    {
        return -EBADPATH;
    }
    int32_t drive_no = to_digit(*path[0]);
    *path += 3; // advance to next path part
    return drive_no;
}

static struct path_root* pathparser_create_root(int32_t drive_number)
{
    struct path_root *root = kzalloc(sizeof(struct path_root));   
    root->drive_no = drive_number;
    root->first = 0;
    return root;
}

static const asciistring pathparser_get_part(asciistring* path)
{
    asciistring result_path_part = kzalloc(ERUOS_MAX_PATH);
    int32_t i = 0;
    while (**path != '/' && **path != '\0')
    {
       result_path_part[i] = **path; 
       *path += 1;
       ++i;
    }

    if (**path == '/')
    {
        // skip ahead
        *path += 1;        
    }

    if (i==0)
    {
        kfree(result_path_part);
        result_path_part = 0;
    }

    return result_path_part;

}

struct path_part* pathparser_parse_part(struct path_part* last_part, asciistring* path)
{
    const asciistring path_part_str = pathparser_get_part(path);
    if (!path_part_str)
    {
        return 0;
    }
    struct path_part* part = kzalloc(sizeof(struct path_part));
    part->part = path_part_str;
    part->next = 0;

    if (last_part)
    {
        last_part->next = part;
    }
    return part;
}

void pathparser_free(struct path_root* root)
{
    struct path_part* part = root->first;
    while (part)
    {
        struct path_part* next_part = part->next;
        kfree((mem_ptr)(part->part));
        kfree(part);
        part = next_part;
    }
    kfree(root);
}

struct path_root* pathparser_parse(const asciistring path, const asciistring current_dir_path)
{
    if (strlen(path) > ERUOS_MAX_PATH)
    {
        return 0;
    }

    asciistring tmp_path = path;
    int32_t drive = pathparser_get_drive(&tmp_path);
    if (drive < 0)
    {
        return 0;
    }

    struct path_root* root = pathparser_create_root(drive);
    struct path_part* first_part = pathparser_parse_part(0,&tmp_path);
    if (!first_part)
    {
        return root;
    }
    root->first = first_part;
    struct path_part* part = pathparser_parse_part(first_part,&tmp_path);
    while (part)
    {
        part = pathparser_parse_part(first_part,&tmp_path);
    }

    return root;
}