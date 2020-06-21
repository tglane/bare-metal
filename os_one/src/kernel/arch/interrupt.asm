[GLOBAL idt_flush]                  ; make function accessible to c/c++ functions
idt_flush:                          ; load interrupt descriptor table described in kernel/arch/interrupt
    mov eax, [esp + 4]
    lidt [eax]
    ret

%macro ISR_NO_ERRORCODE_HANDLER 1        ; define macro with one parameter
[GLOBAL isr%1]                  ; %1 accesses the first parameter
isr%1:
    cli                         ; disable interrupts
    push byte 0                 ; push dummy error code (isr pushes no error code)
    push byte %1                ; push interrupt number
    jmp isr_common_stub
%endmacro

%macro ISR_ERRORCODE_HANDLER 1
[GLOBAL isr%1]
isr%1:
    cli
    push byte %1
    jmp isr_common_stub
%endmacro


; isr handler entries using macros
ISR_NO_ERRORCODE_HANDLER 0
ISR_NO_ERRORCODE_HANDLER 1
ISR_NO_ERRORCODE_HANDLER 2
ISR_NO_ERRORCODE_HANDLER 3
ISR_NO_ERRORCODE_HANDLER 4
ISR_NO_ERRORCODE_HANDLER 5
ISR_NO_ERRORCODE_HANDLER 6
ISR_NO_ERRORCODE_HANDLER 7
ISR_ERRORCODE_HANDLER 8
ISR_NO_ERRORCODE_HANDLER 9
ISR_ERRORCODE_HANDLER 10
ISR_ERRORCODE_HANDLER 11
ISR_ERRORCODE_HANDLER 12
ISR_ERRORCODE_HANDLER 13
ISR_ERRORCODE_HANDLER 14
ISR_NO_ERRORCODE_HANDLER 15
ISR_NO_ERRORCODE_HANDLER 16
ISR_NO_ERRORCODE_HANDLER 17
ISR_NO_ERRORCODE_HANDLER 18
ISR_NO_ERRORCODE_HANDLER 19
ISR_NO_ERRORCODE_HANDLER 20
ISR_NO_ERRORCODE_HANDLER 21
ISR_NO_ERRORCODE_HANDLER 22
ISR_NO_ERRORCODE_HANDLER 23
ISR_NO_ERRORCODE_HANDLER 24
ISR_NO_ERRORCODE_HANDLER 25
ISR_NO_ERRORCODE_HANDLER 26
ISR_NO_ERRORCODE_HANDLER 27
ISR_NO_ERRORCODE_HANDLER 28
ISR_NO_ERRORCODE_HANDLER 29
ISR_NO_ERRORCODE_HANDLER 30
ISR_NO_ERRORCODE_HANDLER 31


[EXTERN isr_handler]        ; defined in isr.cc


; common isr stub
; saves the processor state, sets up for kernel mode segments and calls c/c++ level handler and finally restores stack
isr_common_stub:
    pusha                   ; pops edi, esi, ebp, esp, ebx, edx, ecx, eax

    mov ax, ds
    push eax                ; save data segment desriptor

    mov ax, 0x10            ; load kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call isr_handler

    pop eax                 ; reload original data segement descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa                    ; pops edi, esi, ebp, esp, ebx, edx, ecx, eax
    add esp, 8              ; clean up pushed error code and isr number
    sti
    iret                    ; pop cs, eip, eflags, ss and esp at once

