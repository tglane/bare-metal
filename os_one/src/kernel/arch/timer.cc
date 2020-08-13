#include "kernel/arch/timer.h"

#include "kernel/arch/ports.h"

namespace kernel
{

void p_i_timer::init(uint32_t frequency_hz)
{
    m_divisor = 1193180 / frequency_hz;

    // Send command byte
    // 0x43 command port for the timer
    kernel::arch::port_byte_out(TIMER_0_COMMAND, 0x36);

    // Send divisor byte-wise
    kernel::arch::port_byte_out(TIMER_0_DATA, (uint8_t) (m_divisor & 0xff));
    kernel::arch::port_byte_out(TIMER_0_DATA, (uint8_t) ((m_divisor >>  8) & 0xff));
}

void p_i_timer::handle_interrupt(const cpu_register_state& regs)
{
    // m_ticks++;
    // m_writer.write("Tick: ");
    // m_writer.write(m_ticks);
    // m_writer.newline();   
}

}

