#ifndef KERNEL_DEVICE_KEYBOARD_H
#define KERNEL_DEVICE_KEYBOARD_H

#include "kernel/arch/isr.h"

namespace kernel
{

class ps2_keyboard : public InterruptHandler
{
public:
    static ps2_keyboard& instance()
    {
        static ps2_keyboard inst;
        return inst;
    }

    void handle_interrupt(const cpu_register_state& regs) override;

private:
    ps2_keyboard()
        : InterruptHandler(IRQ1)
    {}

};

}

#endif

