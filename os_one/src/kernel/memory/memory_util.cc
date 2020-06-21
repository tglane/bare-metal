#include "kernel/memory/memory_util.h"

namespace kernel {

size_t strlen(const char* string)
{
    size_t len = 0;
    while(string[len])
        len++;

    return len;
}

void* memcpy(void* dest, const void* src, size_t size)
{
    unsigned char* dest_ptr = (unsigned char*) dest;
    unsigned char* src_ptr = (unsigned char*) src;

    while(size--)
        *dest_ptr++ = *src_ptr++;

    return dest;
}

void* memset(void* dest, uint32_t val, size_t size)
{
    unsigned char* dest_ptr = (unsigned char*) dest;

    while(size--)
        *dest_ptr++ = (unsigned char) val;

    return dest;
}

}

