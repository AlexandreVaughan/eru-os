#include "string.h"

size_t strlen(const asciistring str)
{
    size_t len = 0;
    asciistring ptr = str;
    while (ptr && *ptr != '\0')
    {
        ++len;
        ++ptr;
    }
    return len;
}

size_t strnlen(const asciistring str, size_t max_size)
{
    size_t len = 0;
    for (len=0; len < max_size; ++len)
    {
        if (str[len] == 0)
            return len;
    }
    return len;
}

bool is_digit(asciichar c)
{
    return c >= 48 && c <= 57;
}

int32_t to_digit(asciichar c)
{
    return c - 48;
}