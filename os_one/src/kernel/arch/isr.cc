#include "kernel/arch/isr.h"

#include "kernel/arch/idt.h"
#include "drivers/ports.h"
#include "drivers/text_mode.h"

namespace kernel
{

interrupt_handler::interrupt_handler(uint32_t nr)
    : m_interrupt_nr(nr)
{
    register_interrupt_handler(nr, this);
}

interrupt_handler* handler_table[256];

void register_interrupt_handler(uint8_t n, interrupt_handler* handler)
{
    handler_table[n] = handler;
}

extern "C" void isr_handler(cpu_register_state regs)
{
    drivers::TextModeWriter& isr_writer = drivers::TextModeWriter::instance();
    isr_writer.write("received interrupt: ");
    isr_writer.write(regs.int_no);
    isr_writer.newline();
}

extern "C" void irq_handler(cpu_register_state regs)
{
    if(handler_table[regs.int_no] != 0)
        handler_table[regs.int_no]->handle_interrupt(regs);

    // Send end of interrupt (EOI) signal to the PICs
    if(regs.int_no >= 40)
        drivers::port_byte_out(PIC2_COMMAND, PIC_EOI);
    drivers::port_byte_out(PIC1_COMMAND, PIC_EOI);
}

}

