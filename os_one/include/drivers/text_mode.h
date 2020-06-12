#ifndef DRIVERS_TEXT_MODE_H
#define DRIVERS_TEXT_MODE_H

#include <stdint.h>
#include <stddef.h>

#include "kernel/device/vga_buffer.h"
#include "kernel/memory/memory_util.h"

namespace drivers {

class TextModeWriter
{
public:

    explicit TextModeWriter();

    void write(const char* string, size_t len);

    void write(const char* string);

    void set_cursor(uint32_t x, uint32_t y)
    {
        m_x = x;
        m_y = y;
    }

    void set_color(kernel::vga_color fg, kernel::vga_color bg);

    void newline();

    void clear();

    uint32_t get_cursor_x() { return m_x; }
    
    uint32_t get_cursor_y() { return m_y; }

    void update();

private:

    void put_entry(char c, uint8_t color, uint32_t x, uint32_t y);

    kernel::VgaBuffer* m_vga = &kernel::VgaBuffer::instance();

    uint16_t m_buffer[VGA_WIDTH * VGA_HEIGHT];
    const uint32_t m_col = VGA_WIDTH, m_row = VGA_HEIGHT;

    uint32_t m_x = 0, m_y = 0;

    uint8_t m_color = kernel::vga_color::VGA_COLOR_WHITE | kernel::vga_color::VGA_COLOR_BLACK << 4; 

};

}

#endif

