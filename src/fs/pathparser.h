#ifndef PATHPARSER_H
#define PATHPARSER_H

#include <stdint.h>
#include "types.h"

struct path_root
{
    int32_t drive_no;
    struct path_part* first;
};

struct path_part
{
    const asciichar* part;
    struct path_part* next;
};

void pathparser_free(struct path_root* root);
struct path_root* pathparser_parse(const asciistring path, const asciistring current_dir_path);


#endif
