#include "kernel/arch/ports.h"

namespace kernel { namespace arch
{

uint8_t port_byte_in(uint16_t port)
{
    uint8_t res;
    /* Inline assembly syntax:
     * "=a (res)" set the variable res to the value of register eax
     * "d" (port) map variable port into register edx
     */
    asm("in %%dx, %%al" : "=a" (res) : "d" (port));
    return res;
}

void port_byte_out(uint16_t port, uint8_t data)
{
    asm("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint16_t port_word_in(uint16_t port)
{
    uint16_t res;
    asm("in %%dx, %%ax" : "=a" (res) : "d" (port));
    return res;
}

void port_word_out(uint16_t port, uint16_t data)
{
    asm("out %%ax, %%dx" : : "a" (data), "d" (port));
}

}}
