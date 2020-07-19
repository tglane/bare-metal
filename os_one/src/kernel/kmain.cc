#include <stdint.h>

#include "kernel/device/vga_buffer.h"
#include "drivers/text_mode.h"
#include "kernel/memory/memory_util.h"
#include "kernel/arch/idt.h"
#include "kernel/arch/timer.h"

using drivers::TextModeWriter;
using kernel::p_i_timer;

extern "C" int main()
{
    init_idt();

    TextModeWriter& t = TextModeWriter::instance();
    t.write("Starting kernel init\n");

    // asm volatile("int $0x4");

    // t.write("\nAfter interrupt");
    // p_i_timer::instance().init(50);

    while(1);
    return 0; 
}

