; Jumpt to kernel main function
[global load_kernel]
[bits 32]

[extern main]
section .text.entry

load_kernel:
    call main               ; call main kernel function
    jmp $
