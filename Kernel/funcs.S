.globl vga_write2
vga_write2:
    push %ebp
    movl %esp, %ebp

    push %ecx //color
    push %edx //linea
    push %eax //mensaje
    call vga_write

    leave
    ret