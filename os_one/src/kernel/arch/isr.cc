#include "kernel/arch/isr.h"

#include "drivers/text_mode.h"

extern "C" {

void isr_handler(isr_cpu_state cpu_state)
{
    drivers::TextModeWriter t;
    t.write("received interrupt: ");
    t.write(cpu_state.int_no);
}

}

