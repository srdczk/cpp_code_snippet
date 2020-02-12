; 初始化工作 
[org 0x7c00]
    ; kernel 读取到 的 起始地址
    load_addr equ 0x1000

    ; 初始化工作
entry:
    mov ax, 0x00
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov si, ax

readFloppy:
    ; 从 二号 扇区开始 读取两个扇区的内容
    mov ch, 0x00; 柱面号
    mov dh, 0x00; 磁头号
    mov cl, 0x02; 扇区号

    mov bx, load_addr

    mov ah, 0x02 ; 读盘操作
    mov al, 0x02 ; 连续读取的扇区数量
    mov dl, 0

    int 0x13

    jc read_error
    ; 直接跳转到kernel 执行
    lgdt [gdt_descriptor]
    cli
    in al, 0x92
    or al, 0x02
    out 0x92, al
    ; 进入保护模式
    mov eax, cr0
    or eax, 0x01
    mov cr0, eax
    jmp dword code_seg: begin_pm
    ; 跳转到保护模式

[bits 32]
begin_pm:
    mov ax, data_seg
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x90000
    mov esp, ebp
    call load_addr
    jmp $

read_error:
    hlt
    jmp read_error
gdt_start: ; don't remove the labels, they're needed to compute sizes and jumps
    ; the GDT starts with a null 8-byte
    dd 0x0 ; 4 byte
    dd 0x0 ; 4 byte

; GDT for code segment. base = 0x00000000, length = 0xfffff
; for flags, refer to os-dev.pdf document, page 36
gdt_code: 
    dw 0xffff    ; segment length, bits 0-15
    dw 0x0       ; segment base, bits 0-15
    db 0x0       ; segment base, bits 16-23
    db 10011010b ; flags (8 bits)
    db 11001111b ; flags (4 bits) + segment length, bits 16-19
    db 0x0       ; segment base, bits 24-31

; GDT for data segment. base and length identical to code segment
; some flags changed, again, refer to os-dev.pdf
gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_end:

; GDT descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; size (16 bit), always one less of its true size
    dd gdt_start ; address (32 bit)

code_seg equ gdt_code - gdt_start
data_seg equ gdt_data - gdt_start
    times 510 - ($ - $$) db 0
    dw 0xaa55
