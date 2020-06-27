#include <stdint.h>

#include "kernel/device/vga_buffer.h"
#include "drivers/text_mode.h"
#include "kernel/memory/memory_util.h"
#include "kernel/arch/idt.h"
#include "kernel/arch/timer.h"

using drivers::TextModeWriter;
using kernel::Timer;

extern "C" int main()
{
    init_idt();

    TextModeWriter& t = TextModeWriter::instance();
    t.write("Starting kernel init\n");

    asm volatile ("int $0x3");

    //t.write("\nAfter interrupt");
    // Timer::instance().init(100);

    while(1);
    return 0; 
}

