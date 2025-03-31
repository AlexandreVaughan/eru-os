#include "memory.h"

mem_ptr memset(mem_ptr ptr, mem_byte_value val, size_t size)
{
    byte_array byte_ptr = (byte_array)ptr;
    for (int i = 0; i < size; ++i)
    {
        byte_ptr[i]=val;
    }
    return ptr;
}