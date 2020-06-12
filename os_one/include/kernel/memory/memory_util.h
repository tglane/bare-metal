#ifndef KERNEL_MEMORY_MEMORY_UTIL_H
#define KERNEL_MEMORY_MEMORY_UTIL_H

#include <stdint.h>
#include <stddef.h>

namespace kernel {

size_t strlen(const char* string);

void* memcpy(void* dest, const void* src, size_t size);

void* memset(void* dest, uint32_t val, size_t size);

}

#endif

