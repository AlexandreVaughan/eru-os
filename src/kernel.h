#ifndef KERNEL_H
#define KERNEL_H

#include "types.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

void kernel_main();

void print(const asciistring str);

#endif