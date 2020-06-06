#include <stdint.h>

template<typename T>
T do_stuff(T&& t)
{
    return t;
}

extern "C" void main()
{
    uint16_t* video_buffer = (uint16_t*) 0xb8000;
    uint8_t color = 15 | 0 << 4;
    video_buffer[0] = (uint16_t) do_stuff<unsigned char>('f') | (uint16_t) color << 8;
}

