#include "kernel/device/keyboard.h"

#include "drivers/text_mode.h"

namespace kernel
{

ps2_keyboard& kb = ps2_keyboard::instance();

void ps2_keyboard::handle_interrupt(const cpu_register_state& regs)
{
    drivers::TextModeWriter& w = drivers::TextModeWriter::instance();
    w.write("keyboard");
}

}

