#ifndef KERNEL_DEVICE_VGA_BUFFER_H
#define KERNEL_DEVICE_VGA_BUFFER_H

#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

namespace kernel {

enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROW = 14,
    VGA_COLOR_WHITE = 15,
};

class VgaBuffer
{
public:
    static VgaBuffer& instance()
    {
        static VgaBuffer instance;
        return instance;
    }

    void print_char(char c, uint16_t x, uint16_t y);

    void clear_buffer();

    void set_color(enum vga_color fg, enum vga_color bg);

    uint16_t* get() { return m_video_buffer; };
    
private:
    VgaBuffer() = default;

    uint16_t* m_video_buffer = (uint16_t*) 0xb8000;

    uint8_t m_color = vga_color::VGA_COLOR_WHITE | vga_color::VGA_COLOR_BLACK << 4; 

    const uint8_t m_row = VGA_HEIGHT, m_col = VGA_WIDTH;
};

}

#endif 
