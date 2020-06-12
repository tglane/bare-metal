#include <stdint.h>

#include "kernel/device/vga_buffer.h"
#include "drivers/text_mode.h"
#include "kernel/memory/memory_util.h"

using drivers::TextModeWriter;

extern "C" int main()
{
    TextModeWriter t;

    t.set_cursor(0, 17);
    t.write("Hello World\nlul w");
    t.newline();
    t.write(" asdasdasd");
    t.newline();
    t.set_cursor(0, 22);
    t.write("Hello Again!");
    t.newline();
    t.write("asd");
    t.newline();
    t.write("LUL");
    t.newline();
    t.write("bitte funktioniere");
    t.newline();
    t.write("asdasdasgsgdrg");


    return 0;
}

