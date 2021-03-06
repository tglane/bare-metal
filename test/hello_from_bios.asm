; boot.asm

bits 16                     ; Working in 16 Bit mode now
org 0x7c00                  ; Following code starts at adress 0x7c00

    xor ax, ax              ; Make it zero
    mov ds, ax              ;
    cld                     ;

    mov si, msg             ; Set si to the message to be printed
    call bios_print         ; Call subroutine

hang:
    jmp hang                ; Endless loop at the end

msg     db 'Hello World from Bios', 13, 10, 0

bios_print:
    lodsb                   ; Load byte from si (containing the hello message) into register AL
    or al, al               ; Zero = end of string
    jz done                 ; Get out
    mov ah, 0x0E            ; Write Character in TTY mode
    mov bh, 0               ;
    int 0x10                ; BIOS Interrupt 0x10 -> Video Service
    jmp bios_print          ;
done:
    ret
    
    times 510-($-$$) db 0   ; Zero out remaining boot sector
    db 0x55                 ; Mark end of boot sector
    db 0xaa

