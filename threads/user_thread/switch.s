.section text
.global switch_to

switch_to:

    mov cur, %eax
    mov %esp, task(,%eax,4)
    mov 8(%ebp), %eax
    mov %eax, cur
    mov task(,%eax,4), %esp

    ret


