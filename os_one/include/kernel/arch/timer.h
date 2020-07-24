#ifndef KERNEL_ARCH_TIMER_H
#define KERNEL_ARCH_TIMER_H

#include <stdint.h>
#include "kernel/arch/isr.h"
#include "drivers/text_mode.h"

#define TIMER_0_COMMAND 0x43
#define TIMER_0_DATA 0x40

namespace kernel
{

class p_i_timer : public interrupt_handler
{
public:
    static p_i_timer& instance()
    {
        static p_i_timer instance;
        return instance;
    }

    void init(uint32_t frequency_hz);

    void handle_interrupt(const cpu_register_state& regs) override;

    uint32_t get_ticks() { return m_ticks; }

private:
    p_i_timer()
        : interrupt_handler(IRQ0)
    {}

    drivers::TextModeWriter& m_writer = drivers::TextModeWriter::instance();

    uint32_t m_divisor;

    uint32_t m_ticks = 0;

};

}

#endif

