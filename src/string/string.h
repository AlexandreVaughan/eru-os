#ifndef STRING_H
#define STRING_H

#include "types.h"
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


size_t strlen(const asciistring str);
size_t strnlen(const asciistring str, size_t max_size);
bool is_digit(asciichar c);
int32_t to_digit(asciichar c);

#endif