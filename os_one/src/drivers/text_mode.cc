#include "drivers/text_mode.h"
#include "kernel/arch/ports.h"

namespace drivers {

TextModeWriter::TextModeWriter()
{
    m_vga->clear_buffer();
}

void TextModeWriter::write(const char* string, size_t len)
{
    if(m_x + len > m_col)
    {
        // TODO check for line overflow
    }

    for(uint32_t i = 0; i < len; i++)
    {
        switch(string[i])
        {
            case '\n': 
                this->newline();
                break;
            case '\t': 
                for(uint8_t j = 0; j < m_tabsize; j++)
                    this->put_entry(' ', m_color, m_x++, m_y);
                break;
            default:
                this->put_entry(string[i], m_color, m_x++, m_y);
                break;
        }
    }

    this->update();
}

void TextModeWriter::write(const char* string)
{
    this->write(string, kernel::strlen(string));
}

void TextModeWriter::write(const char c)
{
    this->write(&c, 1);
}

void TextModeWriter::write(uint32_t number)
{
    char nstring[11];
    uint8_t i = 0, j = 0;

    while(number != 0)
    {
        nstring[i++] = (number % 10) + '0';
        number /= 10;
    }
    nstring[i] = '\0';
    i--;
    
    while(j < i)
    {
        char tmp = nstring[j];
        nstring[j++] = nstring[i];
        nstring[i--] = tmp;
    }

    this->write(nstring, kernel::strlen(nstring));
}

void TextModeWriter::write_hex(uint32_t hex_number)
{
    // TODO
}

void TextModeWriter::set_color(kernel::vga_color fg, kernel::vga_color bg)
{
    m_color = fg | bg << 4;
}

void TextModeWriter::newline()
{
    m_x = 0;
    if(m_y + 1 >= m_row)
    {
        kernel::memcpy(&m_buffer[0], &m_buffer[m_col], (m_row - 1) * m_col * sizeof(uint16_t));
        kernel::memset(&m_buffer[m_y * m_col], 0x0000, m_row * sizeof(uint16_t));
    }
    else
    {
        m_y++;
    }
}

void TextModeWriter::clear()
{
    kernel::memset((void*) m_buffer, 0x0000, m_row * m_col);
    m_x = 0;
    m_y = 0;
}

void TextModeWriter::update()
{
    kernel::memcpy((void*) m_vga->get(), (const void*) m_buffer, (m_row * m_col) * sizeof(uint16_t));
}

void TextModeWriter::put_entry(char c, uint8_t color, uint32_t x, uint32_t y)
{
    m_buffer[y * m_col + x] = (uint16_t) c | (uint16_t) color << 8;
}

}

