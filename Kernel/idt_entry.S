#define PIC1 0x20
#define ACK_IRQ 0x20

.globl ack_irq
ack_irq:
    // Indicar que se manejo la interrupcion.
    movl $ACK_IRQ, %eax
    outb %al, $PIC1
    iret


.globl timer_asm
timer_asm:
    // Guardar registros e invocar handler
    pusha
    call timer

    // Ack *antes* de llamar a sched()
    movl $ACK_IRQ, %eax
    outb %al, $PIC1

    // Llamada a sched con argumento
    push %esp
    call sched

    // Retornar (si se volvió de sched)
    addl $4, %esp
    popa
    iret

.globl keyboard_asm
keyboard_asm:
        pusha
        call keyboard
        popa

        jmp ack_irq

.globl divzero
divzero:
    // (1) Guardar registros.

    add $1, %ebx
    push %eax   //caller saved
    push %ecx   //caller saved
    push %edx   //caller saved

    // (2) Preparar argumentos de la llamada.
    //vga_write_cyan("Se divide por ++ebx", 17);

    movl $17, %edx              //linea
    movl $divzero_msg, %eax     //mensaje

    // (3) Invocar a vga_write_cyan()
    call vga_write_cyan

    // (4) Restaurar registros.
    pop %edx
    pop %ecx
    pop %eax

    // (5) Finalizar ejecucion del manejador.
    iret


.globl breakpoint
breakpoint:
    // (1) Guardar registros.
    pusha

    // (2) Preparar argumentos de la llamada.
    //vga_write2("Hello, breakpoint", 14, 0xB0);

    movl $0xB0, %ecx            //color
    movl $14, %edx              //linea
    movl $breakpoint_msg, %eax  //mensaje

    // (3) Invocar a vga_write2()
    call vga_write2

    // (4) Restaurar registros.
    popa

    // (5) Finalizar ejecucion del manejador.
    iret

.data
breakpoint_msg:
    .asciz "Hello, breakpoint"

divzero_msg:
    .asciz "Se divide por ++ebx"
