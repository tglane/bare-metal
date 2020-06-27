[GLOBAL idt_flush]                  ; make function accessible to c/c++ functions
idt_flush:                          ; load interrupt descriptor table described in kernel/arch/interrupt
    mov edx, [esp + 4]
    lidt [edx]
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

%macro IRQ 2
[GLOBAL irq%1]
irq%1:
    cli
    push byte 0
    push byte %2
    jmp irq_common_stub
%endmacro


[EXTERN isr_handler]        ; defined in isr.cc

[EXTERN irq_handler]        ; defined in isr.cc


; common isr stub
; saves the processor state, sets up for kernel mode segments and calls c/c++ level handler and finally restores stack
isr_common_stub:
    pusha                   ; pushes edi, esi, ebp, esp, ebx, edx, ecx, eax

    mov ax, ds              ; lower 16 bits of eax = ds
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


; common irq stub
; same as isr_common_stub
irq_common_stub:
    pusha                   ; pushes edi, esi, ebp, esp, ebx, edx, ecx, eax to the stack

    mov ax, ds              ; lower 16 bits of eax = ds
    push eax                ; save the data segment descriptor

    mov ax, 0x10            ; load kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call irq_handler

    pop ebx                 ; reload original data segment descriptor
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    
    popa
    add esp, 8              ; clean up pushed error code and isr number
    sti
    iret


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


; irq handler entries using macros
IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

