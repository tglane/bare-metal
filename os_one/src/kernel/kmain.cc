#include <stdint.h>

#include "kernel/device/vga_buffer.h"
#include "drivers/text_mode.h"
#include "kernel/memory/memory_util.h"

using drivers::TextModeWriter;

extern "C" int main()
{
    TextModeWriter t;

    t.write("Starting kernel init");
  
    // asm volatile ("int $0x3");

    while(1);
    return 0; 
}

