#ifndef KERNEL_ARCH_ISR_H
#define KERNEL_ARCH_ISR_H

#include <stdint.h>
#include "drivers/text_mode.h"

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

namespace kernel
{

struct __attribute__((packed)) cpu_register_state
{
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};


class InterruptHandler
{
public:
    InterruptHandler(uint32_t nr);

    virtual void handle_interrupt(cpu_register_state regs)
    {
        m_w.write("lül?");
    }

private:
    uint32_t m_interrupt_nr;
    drivers::TextModeWriter& m_w = drivers::TextModeWriter::instance();
};


extern "C" {

void register_interrupt_handler(uint8_t n, InterruptHandler* handler);

void isr_handler(cpu_register_state regs);

void irq_handler(cpu_register_state regs);

}

}

#endif

