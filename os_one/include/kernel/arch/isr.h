#ifndef KERNEL_ARCH_ISR_H
#define KERNEL_ARCH_ISR_H

#include <stdint.h>

extern "C" {

struct __attribute__((packed)) isr_cpu_state
{
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

void isr_handler(isr_cpu_state cpu_state);

}

#endif

