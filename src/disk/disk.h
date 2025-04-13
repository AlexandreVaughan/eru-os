#ifndef DISK_H
#define DISK_H

#include <stdint.h>
#include <stddef.h>
#include "types.h"
#include "status.h"

ret_code disk_read_sector(uint32_t lba, size_t total, mem_ptr buf);

#endif