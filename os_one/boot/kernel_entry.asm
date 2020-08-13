; Jumpt to kernel main function
[global load_kernel]
[bits 32]

[extern main]
;[extern __bss_start]
;[extern __bss_sizel]

section .text.entry

load_kernel:
    ; zero out the bss memory area since it is not guaranteed to already by zero
    xor eax, eax
    ;mov ecx, __bss_sizel
    ;mov edi, __bss_start

    call main               ; call main kernel function
    jmp $
