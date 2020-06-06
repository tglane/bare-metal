[org 0x7c00]                ; offset adresses by 0x7c00
KERNEL_OFFSET equ 0x1000    ; memory offset where kernel will be loaded later

    mov [BOOT_DRIVE], dl    ; BIOS stores boot drive in dl, store it for later

    mov bp, 0x9000          ; set the initial stack bottom (bp = stack base register)
    mov sp, bp              ; set the stack pointer to the base (sp = stack pointer register)
    
    mov bx, MSG_REAL_MODE
    call print_string       ; print message from realmode
    
    call load_kernel        ; load kernel image using BIOS functions

    call switch_prot_mode   ; immediately enter protected mode
    
    jmp $                   ; switch_to_prot_mode never returns


[bits 16]
; function to print a null-terminated string from bx to the screen
print_string:
    pusha
    mov ah, 0x0e
print_loop:
    mov al, [bx]
    cmp al, 0
    je done_print_string
    
    int 0x10
    add bx, 1
    jmp print_loop
done_print_string:
    mov al, 0xa
    int 0x10
    mov al, 0xd
    int 0x10
    popa
    ret


; function to print an 16 bit hexadecimal adress to the screen
print_hex_16:
    pusha
    mov cx, 0               ; index variable

hex_loop:
    cmp cx, 4
    jge done_print_hex

    mov ax, dx              ; use ax as working directory - hex adress is stored in dx
    and ax, 0x000f          ; masking firt three entries out
    add al, 0x30              ; convert to ascii by adding decimal 30
    cmp al, 0x39            ; check if its a number or a char
    jle inner_loop
    add al, 7               ; char needs add of 7

inner_loop:
    mov bx, HEX_STRING
    add bx, 5
    sub bx, cx
    mov [bx], al
    shr dx, 4               ; ring shift string in dx so that the next char is on the last position and gets masked
    add cx, 1               ; increment loop couner
    jmp hex_loop

done_print_hex:
    mov bx, HEX_STRING
    call print_string
    popa
    ret

HEX_STRING:
    db '0x0000', 0


; function to load dh sectors to es:bx from drive dl
disk_load:
    push dx         ; store dx on the stack so later the amount of sectors that were
                    ; requested to be read can be checked
    mov ah, 0x02    ; BIOS read sectors function
    mov al, dh      ; amount of sectors is stored in dh
    mov ch, 0x00    ; select disk cylinder 0
    mov dh, 0x00    ; select disk head 0
    mov cl, 0x02    ; start reading from second sector (behind the boot sector)
    int 0x13        ; BIOS interrupt for reading from disk
    
    jc disk_error   ; jump if dist error occured (carry flag is set)
    
    pop dx
    cmp dh, al      ; if al (sectors read) != dh (sectors expected) show error
    jne disk_error
    
    ret

disk_error:
    mov bx, DISK_ERROR_MSG
    call print_string
    jmp $
DISK_ERROR_MSG db "Disk read error", 0


; load kernel image from disk using BIOS routines
load_kernel:
    mov bx, MSG_LOADING_KERNEL
    call print_string

    mov bx, KERNEL_OFFSET           ; bx stores the target adress to load the data into
    mov dh, 15                      ; dh stores the number of sectors to read
    mov dl, [BOOT_DRIVE]            ; dl stores the disk to read from
    call disk_load

    ret


; GDT description
; GDT contains segmentation information for the 32 bit protected mode
gdt_start:

gdt_null:           ; the mandatory null descriptor
    dd 0x0          ; dd defines a double word (4 bytes)
    dd 0x0

gdt_code:           ; code segment descriptor
    ; base = 0x0, limit = 0xfffff, 1st flags: 1(present) 00(priviledge) 1(descriptor type) -> 1001b
    ; type flags: 1(code) 0(conforming) 1(readable) 0(accessed) -> 1010b
    ; 2nd flags: 1(granularity) 1(32-bit default) 0(64-bit segment) 0(AVL) -> 1100b
    dw 0xffff       ; limit (bits 0 - 15)
    dw 0x0          ; base (bits 0 - 15)
    db 0x0          ; base (bits 16 - 23)
    db 10011010b    ; 1st flags and type flags
    db 11001111b    ; 2nd flags and limit (bits 16 - 19)
    db 0x0          ; base (bits 24 - 31)

gdt_data:           ; data segment descriptor
    ; same structure as code segment except for the type flags:
    ; type flags: 0(code) 0(expand down) 1(writeable) 0(accessed) -> 0010b
    dw 0xffff       ; limit (bits 0 - 15)
    dw 0x0          ; base (bits 0 - 15)
    db 0x0          ; base (bits 16 - 23)
    db 10010010b    ; 1st flags and type flags
    db 11001111b    ; 2nd flags and limit (bits 16 - 19)
    db 0x0          ; base (bits 24 - 31)

gdt_end:            ; define ending label to calculate the size of the complete gdt

gdt_descriptor:     ; gdt descriptor
    dw gdt_end - gdt_start          ; size of the gdt structure above, always one less than the true size
    dd gdt_start                    ; start address of the gdt structure


; constants for the gdt descriptor offset
; segment registers must contain these when in protected mode 
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start


; function to switch to 32 bit protected mode
switch_prot_mode:
    cli                             ; disable interrupts 
                                    ; old IVT set up by the BIOS is not usable when in protected mode
                                    ; because its calling 16 bit real mode function with wrong segmentation
    lgdt [gdt_descriptor]           ; load global descriptor table described above

    mov eax, cr0                    ; perform swith to protected mode 
    or eax, 0x1                     ; setting the protected mode bit (1st bit) in cr0 control register
    mov cr0, eax                    ;

    jmp CODE_SEG:init_prot_mode     ; perform a far jump (to a new segment) to the 32 bit code
                                    ; forces the cpu to flush the pipeline and caches of pre-fetched 16 bit real-mode code


[bits 32]
; initialize registers and the stack once for the protected mode
init_prot_mode:
    mov ax, DATA_SEG                ; in protected mode old segments are meaningless
    mov ds, ax                      ; therefore point segment registers to the data 
    mov ss, ax                      ; selector defined in the gdt above
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000                ; update stack position so it is at the top of the free space
    mov esp, ebp

    call begin_prot_mode


; function to print a null-terminated string from ebx in protected mode
print_string_pm:
    pusha
    mov edx, 0xb8000                ; set edx to start address of video memory

print_string_pm_loop:
    mov al, [ebx]                   ; store char at ebx in al 
    mov ah, 0x0f                    ; store color attributes in ah
    cmp al, 0                       ; jump the function end if end of string reached
    je done_print_string_pm

    mov [edx], ax                   ; store char and attributs at current character cell of video memory
    add ebx, 1                      ; increment ebx to nect char in string
    add edx, 2                      ; move to next cell in video memory
    jmp print_string_pm_loop

done_print_string_pm:
    popa
    ret


; label is called after the switch to the protected mode
begin_prot_mode:
    mov ebx, MSG_PROT_MODE
    call print_string_pm
    
    call KERNEL_OFFSET

    jmp $                           ; hang

; global variables
BOOT_DRIVE db 0
MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_PROT_MODE db "Successfully switched to 32-bit Protected Mode", 0
MSG_LOADING_KERNEL db "Loading kernel into memory", 0

times 510 - ($-$$) db 0
dw 0xaa55

