#include "kernel/device/vga_buffer.h"

namespace kernel {

void VgaBuffer::print_char(char c, uint16_t x, uint16_t y)
{
    m_video_buffer[y * m_col + x] = (uint16_t) c | (uint16_t) m_color << 8;
}

void VgaBuffer::clear_buffer()
{
    uint8_t tmp = m_color;
    m_color = (uint8_t) 0b00000000;

    for(uint8_t i = 0; i < m_row; i++)
    {
        for(uint8_t j = 0; j < m_col; j++)
            print_char(' ', j, i);
    }

    m_color = tmp;
}

void VgaBuffer::set_color(enum vga_color fg, enum vga_color bg)
{
    m_color = fg | bg << 4;
}

}

