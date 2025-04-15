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

int32_t memcmp(mem_ptr s1, mem_ptr s2, size_t count)
{
    byte_array c1 = (byte_array)s1;
    byte_array c2 = (byte_array)s2;
    
    while(count-- > 0)
    {
        if (*c1++ != *c2++)
        {
            return c1[-1] < c2[-1] ? -1 : 1;
        }
    }

    return 0;

}