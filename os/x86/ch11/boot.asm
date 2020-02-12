

    mov ax, cs
    mov ss, ax
    mov sp, 0x7c00

    ; 初始化 栈 空间

    ; 初始化 bx 寻址
    mov ds, ax
    mov bx, 0x7e00

    ; 初始化 gdt 表
    ; 0 空 描述 符
    ; 双字 32 位 + 32 位
    mov dword [bx + 0x00], 0x00
    mov dword [bx + 0x04], 0x00

    ; 创建 代码段 描述符
    ; 段 基址 0x7c00, 段界限0x1ff
    mov dword [bx + 0x08], 0x7c0001ff
    mov dword [bx + 0x0c], 0x00409800

    ; 数据段 描述符
    ; 基址 0xb8000 -> 显存的起始地址
    mov dword [bx + 0x10], 0x8000ffff
    mov dword [bx + 0x14], 0x0040920b

    ; 栈段描述符
    mov dword [bx + 0x18], 0x00007a00
    mov dword [bx + 0x1c], 0x00409600

    ; gdt 表 的大小
    mov word [gdt_size + 0x7c00], 31

    lgdt [gdt_size + 0x7c00]
    
    ; 取出 92 端口的数据
    ; 与 0x02 或操作
    in al, 0x92
    or al, 0x02
    out 0x92, al

    cli

    mov eax, cr0
    or eax, 1
    mov cr0, eax
    ; 段选择子
    jmp dword 0x0008: flush

[bits 32]
flush:
    mov cx, 0x10
    mov ds, cx
    ; 可以知道 print 函数 的实现 -> 
    mov byte [0x07ca], 'N'
    mov byte [0x07cb], 0x0c
    mov byte [0x07cc], 'M'
    mov byte [0x07cd], 0x0c
    mov byte [0x07ce], 'A'
    mov byte [0x07cf], 0x0c
    mov byte [0x07d0], 'S'
    mov byte [0x07d1], 0x0c
    mov byte [0x07d2], 'I'
    mov byte [0x07d3], 0x0c
    mov byte [0x07d4], 'L'
    mov byte [0x07d5], 0x0c
    mov byte [0x07d6], 'E'
    mov byte [0x07d7], 0x0c

    mov cx, 0x18
    mov ss, cx
    mov esp, 0x7c00
    mov ebp, esp

    jmp $

gdt_size:
    dw 0
gdt_base:
    dd 0x00007e00

    times 510 - ($ - $$) db 0
    dw 0xaa55




