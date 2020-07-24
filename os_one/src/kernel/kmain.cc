#include <stdint.h>

#include "kernel/device/vga_buffer.h"
#include "drivers/text_mode.h"
#include "kernel/memory/memory_util.h"
#include "kernel/arch/idt.h"
#include "kernel/arch/timer.h"
#include "kernel/device/keyboard.h"

using drivers::TextModeWriter;
using kernel::p_i_timer;
using kernel::ps2_keyboard;

void init_interrupt_handler()
{
    p_i_timer::instance().init(50);
    ps2_keyboard::instance().init();
}

extern "C" int main()
{
    init_idt();
    init_interrupt_handler();

    TextModeWriter& t = TextModeWriter::instance();
    t.write("Starting kernel init\n");

    // asm volatile("int $0x4");

    while(1);
    return 0; 
}

