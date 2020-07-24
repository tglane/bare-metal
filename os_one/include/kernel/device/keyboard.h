#ifndef KERNEL_DEVICE_KEYBOARD_H
#define KERNEL_DEVICE_KEYBOARD_H

#include "kernel/arch/isr.h"
#include "drivers/ports.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_COMMAND_PORT 0x64

namespace kernel
{

class ps2_keyboard : public interrupt_handler
{
public:
    static ps2_keyboard& instance()
    {
        static ps2_keyboard inst;
        return inst;
    }

    void init();

    void handle_interrupt(const cpu_register_state& regs) override;

private:
    ps2_keyboard()
        : interrupt_handler(IRQ1)
    {}

    bool m_caps = false;
    bool m_ctrl = false;
};

}

#endif

