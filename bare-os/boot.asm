section .boot
bits 16                             ; Working in 16 bit mode now
global boot
boot:
    mov ax, 0x2401                  ; Activate A20 Line
    int 0x15                        ; To adress more than 1 MB of memory

    mov ax, 0x3                     ;
    int 0x10                        ; Set VGA text mode 3

    mov [disk], dl

                                    ; Read second sector
                                    ; Disk number implicitly placed in dl
    mov ah, 0x2                     ; Read sectors
    mov al, 6                       ; Sectors to read
    mov ch, 0                       ; Cylinder idx
    mov dh, 0                       ; Head idx
    mov cl, 2                       ; Sector idx
    mov dl, [disk]                  ; Disk idx
    mov bx, copy_target             ; Target pointer
    int 0x13                        ; Interrupt to read sectors from a drive to target location
    cli 

    lgdt [gdt_pointer]              ;
    mov eax, cr0                    ;
    or eax, 0x1                     ;
    mov cr0, eax                    ;
    mov ax, DATA_SEG                ; 
    mov ds, ax                      ;
    mov es, ax                      ;
    mov fs, ax                      ;
    mov gs, ax                      ;
    mov ss, ax                      ;
    jmp CODE_SEG:boot2
gdt_start:
    dq 0x0                          ;
gdt_code:
    dw 0xFFFF                       ;
    dw 0x0                          ;
    db 0x0                          ;
    db 10011010b                    ;
    db 11001111b                    ;
    db 0x0                          ;
gdt_data:
    dw 0xFFFF                       ;
    dw 0x0                          ;
    db 0x0                          ;
    db 10010010b                    ;
    db 11001111b                    ;
    db 0x0
gdt_end:
gdt_pointer:
    dw gdt_end - gdt_start          ;
    dd gdt_start                    ;
disk:
    db 0x0                          ;
CODE_SEG equ gdt_code - gdt_start   ;
DATA_SEG equ gdt_data - gdt_start   ;

times 510 - ($-$$) db 0             ; Zero out remaining bootsector
dw 0xaa55                           ; Mark end of bootsector

copy_target:
bits 32                             ; Working in 32 bit mode now
    hello_msg: db "Hello from more than 512 bytes!!",0
boot2:
    mov esi, hello_msg              ; Move hello world message in esi
    mov ebx, 0xb8000                ;
.loop:
    lodsb                           ; Read character from esi to al
    or al, al                       ; check if al = 0 (means that message is read completely)
    jz halt                         ; if al = 0 jump to halt
    or eax, 0x0F00                   ;
    add ebx, 2                      ;
    jmp .loop                       ; Read another character from hello_msg

halt:
    mov esp, kernel_stack_top       ;
    extern kmain              ; Load file containing kernel main function
    call kmain                ; Call main function from kernel
    cli                             ; Clear interrupts
    hlt                             ;

section .bss
align 4
kernel_stack_bottom: equ $
    resb 16384                      ; 16 KB
kernel_stack_top:

