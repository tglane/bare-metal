#ifndef KERNEL_ARCH_TIMER_H
#define KERNEL_ARCH_TIMER_H

#include <stdint.h>
#include "kernel/arch/isr.h"
#include "drivers/text_mode.h"

#define TIMER_0_COMMAND 0x43
#define TIMER_0_DATA 0x40

namespace kernel
{

class Timer : public InterruptHandler
{
public:
    static Timer& instance()
    {
        static Timer instance;
        return instance;
    }

    void init(uint32_t frequency);

    void handle_interrupt(cpu_register_state regs) override;

private:
    Timer()
        : InterruptHandler(IRQ0)
    {}

    drivers::TextModeWriter& m_writer = drivers::TextModeWriter::instance();

    uint32_t m_divisor;

    uint32_t m_ticks;

};

}

#endif

