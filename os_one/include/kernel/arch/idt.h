#ifndef KERNEL_ARCH_IDT_H
#define KERNEL_ARCH_IDT_H

#include <stdint.h>

#include "kernel/memory/memory_util.h"

extern "C"{

struct __attribute__((packed)) IDT_Ptr
{
    uint16_t limit;
    uint16_t base;          // Address of first element of the idt
};

struct __attribute__((packed)) IDT_Entry
{
    uint16_t offset_low;    // Lower 16 bits of the address to jump to when its interrupt fires
    uint16_t selector;      // Kernel segment selector
    uint8_t zero;           // This must ALWAYS be zero
    uint8_t flags;          // Flags. See documentation
    uint16_t offset_high;   // Upper 16 bits of the address to jump to
};

// Extern directives let us access the addresses of the assembly ISR handlers
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

}

#endif

