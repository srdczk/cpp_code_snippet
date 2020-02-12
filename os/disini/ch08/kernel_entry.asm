[bits 32]
[extern main]
    call main
    jmp $

io_hlt:
    hlt
    ret
