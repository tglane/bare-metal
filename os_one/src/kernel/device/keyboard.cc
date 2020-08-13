#include "kernel/device/keyboard.h"

#include "drivers/text_mode.h"

namespace kernel
{

void ps2_keyboard::init()
{
    while(kernel::arch::port_byte_in(KEYBOARD_COMMAND_PORT) & 0x1)
        kernel::arch::port_byte_in(KEYBOARD_DATA_PORT);

    kernel::arch::port_byte_out(KEYBOARD_COMMAND_PORT, 0xae); // activate interrupts
    kernel::arch::port_byte_out(KEYBOARD_COMMAND_PORT, 0x20); // read controller command byte
    uint8_t status = (kernel::arch::port_byte_in(KEYBOARD_DATA_PORT) | 1) & ~0x10;
    kernel::arch::port_byte_out(KEYBOARD_COMMAND_PORT, 0x60); // set controller command byte
    kernel::arch::port_byte_out(KEYBOARD_DATA_PORT, status);
    kernel::arch::port_byte_out(KEYBOARD_DATA_PORT, 0xf4);
}

void ps2_keyboard::handle_interrupt(const cpu_register_state& regs)
{
    uint8_t scancode = kernel::arch::port_byte_in(KEYBOARD_DATA_PORT);
    char c;

    // TODO improve scancode handling (add missing codes, make keyboard layout exchangeable)
    if(scancode & 0x80)
    {
        // Key released
        switch(scancode)
        {
            // TODO release l shift not working ...
            case 0x36:
            case 0x2A: m_caps = !m_caps; break;
            case 0x1D: m_ctrl = !m_ctrl; break;
            default: break;
        }
    }
    else
    {
        // Key pressed
        switch(scancode)
        {
            case 0x02: c = '1'; break;
            case 0x03: c = '2'; break;
            case 0x04: c = '3'; break;
            case 0x05: c = '4'; break;
            case 0x06: c = '5'; break;
            case 0x07: c = '6'; break;
            case 0x08: c = '7'; break;
            case 0x09: c = '8'; break;
            case 0x0A: c = '9'; break;
            case 0x0B: c = '0'; break;
            case 0x0F: c = '\t'; break;

            case 0x10: c = 'q'; break;
            case 0x11: c = 'w'; break;
            case 0x12: c = 'e'; break;
            case 0x13: c = 'r'; break;
            case 0x14: c = 't'; break;
            case 0x15: c = 'z'; break;
            case 0x16: c = 'u'; break;
            case 0x17: c = 'i'; break;
            case 0x18: c = 'o'; break;
            case 0x19: c = 'p'; break;
            case 0x3A: m_caps = !m_caps; break;

            case 0x1E: c = 'a'; break;
            case 0x1F: c = 's'; break;
            case 0x20: c = 'd'; break;
            case 0x21: c = 'f'; break;
            case 0x22: c = 'g'; break;
            case 0x23: c = 'h'; break;
            case 0x24: c = 'j'; break;
            case 0x25: c = 'k'; break;
            case 0x26: c = 'l'; break;

            case 0x2C: c = 'y'; break;
            case 0x2D: c = 'x'; break;
            case 0x2E: c = 'c'; break;
            case 0x2F: c = 'v'; break;
            case 0x30: c = 'b'; break;
            case 0x31: c = 'n'; break;
            case 0x32: c = 'm'; break;
            case 0x33: c = ','; break;
            case 0x34: c = '.'; break;
            case 0x35: c = '-'; break;
            case 0x36: m_caps = !m_caps; break;
            case 0x2A: m_caps = !m_caps; break;

            case 0x1C: c = '\n'; break;
            case 0x39: c = ' '; break;
            case 0x1D: m_ctrl = !m_ctrl; break;

            default: break;
        }

        // TODO improve to upper
        if(m_caps && c) c -= 32;
        if(c) t.write(c);
    }

}

}

