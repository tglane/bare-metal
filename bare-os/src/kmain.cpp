#include <stddef.h>
#include <stdint.h>

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

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str)
{
    size_t len = 0;
    while(str[len])
        len++;
    return len;
}

void* memmove(void* dest_ptr, const void* src_ptr, size_t size)
{
    unsigned char* src = (unsigned char*) src_ptr;
    unsigned char* dest = (unsigned char*) dest_ptr;

    if(dest < src)
    {
        for(size_t i = 0; i < size; i++)
            dest[i] = src[i];
    }
    else
    {
        for(size_t i = size; i > 0; i--)
            dest[i - 1] = src[i - 1];
    }

    return dest_ptr;
}

class terminal 
{
    size_t _max_terminal_row = VGA_HEIGHT;
    size_t _max_terminal_col = VGA_WIDTH;

    size_t _terminal_row = 0;
    size_t _terminal_col = 0;
    uint8_t _terminal_color = VGA_COLOR_BLACK;
    uint16_t* _terminal_buffer = (uint16_t*) 0xb8000;

    terminal()    
    {
        _terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
        for (size_t y = 0; y < _max_terminal_row; y++) 
        {
            for (size_t x = 0; x < _max_terminal_col; x++) 
            {
                const size_t index = y * _max_terminal_col + x;
                _terminal_buffer[index] = vga_entry(' ', _terminal_color);
            }
        }
    }

    void _put_char(char c)
    {
        _put_colored_char(c, _terminal_color);
    }

    void _put_colored_char(char c, uint8_t color)
    {
        if(c == '\n')
            _terminal_col = _max_terminal_col - 1;
        else
            _put_entry_at(c, color, _terminal_col, _terminal_row);

        if(++_terminal_col == _max_terminal_col)
        {
            _terminal_col = 0;

            if(++_terminal_row == _max_terminal_row)
            {
                //_terminal_row = 0;
                _scroll_down();
                _terminal_row--;
            }
        }
    }

    void _put_entry_at(char c, uint8_t color, size_t x, size_t y)
    {
        const size_t index = y * VGA_WIDTH + x;
        _terminal_buffer[index] = vga_entry(c, color);
    }

    void _scroll_down()
    {
        for(size_t y = 0; y < _max_terminal_row; y++)
        {
            for(size_t x = 0; x < _max_terminal_col; x++)
                _terminal_buffer[y * _max_terminal_col + x] = _terminal_buffer[(y + 1) * _max_terminal_col + x];

            //memmove((void*) (_terminal_buffer[y * _max_terminal_col]), 
            //    (void*) (_terminal_buffer[(y + 1) * _max_terminal_col]), _max_terminal_col);
        }
    }

    void _clear_line()
    {
        // TODO
    }

public:
    static terminal& get_instance()
    {
        static terminal _terminal_instance;
        return _terminal_instance;
    }

    void clear()
    {
        for(size_t y = 0; y < _max_terminal_row; y++)
        {
            for(size_t x = 0; x < _max_terminal_col; x++)
            {
                const size_t index = y * _max_terminal_col + x;
                _terminal_buffer[index] = vga_entry(' ', vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
            }
        }
        _terminal_row = 0, _terminal_col = 0;
    }

    void set_color(uint8_t color)
    {
        _terminal_color = color;
    }

    void write(const char* s, size_t size)
    {
        for(size_t i = 0; i < size; i++)
            _put_char(s[i]);
    }

    void write_color(const char* s, size_t size, uint8_t color)
    {
        for(size_t i = 0; i < size; i++)
            _put_colored_char(s[i], color);
    }

    void write_string(const char* s)
    {
        write(s, strlen(s));
    }

    void write_colored_string(const char* s, uint8_t color)
    {
        write_color(s, strlen(s), color);
    }

};

extern "C" void kmain()
{
    terminal& term = terminal::get_instance();
    char text[8] = "helloo\n";

    for(size_t i = 0; i < 30; i ++)
    {
        text[5] = i + 48;
        term.write_string(text);
    }

    // term.write_string("Hello World\n");
    // term.write_colored_string("Hello Again!", VGA_COLOR_BROWN);

    // term.set_color(VGA_COLOR_GREEN);
    // term.write_string("IN GRÜN ALDA!\n");
    // term.write_string("nochmal");
} 

