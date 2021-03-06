#include "kernel/arch/idt.h"

extern "C" {

extern int idt_flush(uint32_t);

static void idt_set_gate(uint8_t, uint32_t, uint16_t = 0x08, uint8_t = 0x8e);

IDT_Entry IDT[256];
IDT_Ptr IDT_ptr;

void init_idt()
{
    IDT_ptr.limit = sizeof(IDT_Entry) * 256 - 1;
    IDT_ptr.base = (uint32_t) &IDT;

    kernel::memset(&IDT, 0, sizeof(IDT_Entry) * 256);
    
    idt_set_gate(0, (uint32_t) isr0);
    idt_set_gate(1, (uint32_t) isr1);
    idt_set_gate(2, (uint32_t) isr2);
    idt_set_gate(3, (uint32_t) isr3);
    idt_set_gate(4, (uint32_t) isr4);
    idt_set_gate(5, (uint32_t) isr5);
    idt_set_gate(6, (uint32_t) isr6);
    idt_set_gate(7, (uint32_t) isr7);
    idt_set_gate(8, (uint32_t) isr8);
    idt_set_gate(9, (uint32_t) isr9);
    idt_set_gate(10, (uint32_t) isr10);
    idt_set_gate(11, (uint32_t) isr11);
    idt_set_gate(12, (uint32_t) isr12);
    idt_set_gate(13, (uint32_t) isr13);
    idt_set_gate(14, (uint32_t) isr14);
    idt_set_gate(15, (uint32_t) isr15);
    idt_set_gate(16, (uint32_t) isr16);
    idt_set_gate(17, (uint32_t) isr17);
    idt_set_gate(18, (uint32_t) isr18);
    idt_set_gate(19, (uint32_t) isr19);
    idt_set_gate(20, (uint32_t) isr20);
    idt_set_gate(21, (uint32_t) isr21);
    idt_set_gate(22, (uint32_t) isr22);
    idt_set_gate(23, (uint32_t) isr23);
    idt_set_gate(24, (uint32_t) isr24);
    idt_set_gate(25, (uint32_t) isr25);
    idt_set_gate(26, (uint32_t) isr26);
    idt_set_gate(27, (uint32_t) isr27);
    idt_set_gate(28, (uint32_t) isr28);
    idt_set_gate(29, (uint32_t) isr29);
    idt_set_gate(30, (uint32_t) isr30);
    idt_set_gate(31, (uint32_t) isr31);

    // Remap the irq table / rempat the pic
    kernel::arch::port_byte_out(PIC1_COMMAND, 0x11); // Start initialization in cascade mode
    kernel::arch::port_byte_out(PIC2_COMMAND, 0x11);
    kernel::arch::port_byte_out(PIC1_DATA, 0x20);    // Master PIC isr offset = 32
    kernel::arch::port_byte_out(PIC2_DATA, 0x28);    // Slave PIC isr offset = 40
    kernel::arch::port_byte_out(PIC1_DATA, 0x04);    // Tell master PIC that there is a slave at IRQ2
    kernel::arch::port_byte_out(PIC2_DATA, 0x02);    // Tell slave PIC its cascade identity
    kernel::arch::port_byte_out(PIC1_DATA, 0x01);    // Master PIC 8086/88 mode
    kernel::arch::port_byte_out(PIC2_DATA, 0x01);    // Slave PIC 8086/88 mode
    kernel::arch::port_byte_out(PIC1_DATA, 0x0);     // No older masks
    kernel::arch::port_byte_out(PIC2_DATA, 0x0);     // No older masks

    idt_set_gate(32, (uint32_t) irq0);
    idt_set_gate(33, (uint32_t) irq1);
    idt_set_gate(34, (uint32_t) irq2);
    idt_set_gate(35, (uint32_t) irq3);
    idt_set_gate(36, (uint32_t) irq4);
    idt_set_gate(37, (uint32_t) irq5);
    idt_set_gate(38, (uint32_t) irq6);
    idt_set_gate(39, (uint32_t) irq7);
    idt_set_gate(40, (uint32_t) irq8);
    idt_set_gate(41, (uint32_t) irq9);
    idt_set_gate(42, (uint32_t) irq10);
    idt_set_gate(43, (uint32_t) irq11);
    idt_set_gate(44, (uint32_t) irq12);
    idt_set_gate(45, (uint32_t) irq13);
    idt_set_gate(46, (uint32_t) irq14);
    idt_set_gate(47, (uint32_t) irq15);

    idt_flush((uint32_t) &IDT_ptr);
}

static void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags)
{
    IDT[num].offset_low = base & 0xffff;
    IDT[num].offset_high = (base << 16) & 0xffff;
    IDT[num].selector = selector;
    // Uncomment when implementing user mode
    // Sets the interrupt gate's priviledge to level 3
    IDT[num].flags = flags /* | 0x60 */;
}

}

