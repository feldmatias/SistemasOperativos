// boot.S

#include "multiboot.h"

#define KSTACK_SIZE 8192

.align 4
multiboot:
    .long MULTIBOOT_HEADER_MAGIC
    .long 0
    .long -(MULTIBOOT_HEADER_MAGIC)

.globl _start
_start:
    // Paso 1: Configurar el stack antes de llamar a kmain.
    movl $0, %ebp
    movl $kstack_end, %esp
    push %ebp

    // Paso 2: pasar la informacion multiboot a kmain. Si el
    // kernel no arranco via Multiboot, se debe pasar NULL.
    movl $0, %ecx
    CMP $MULTIBOOT_BOOTLOADER_MAGIC, %eax
    cmove %ebx, %ecx
    push %ecx
    
    // Usar una instruccion de comparacion (TEST o CMP) para
    // comparar con MULTIBOOT_BOOTLOADER_MAGIC, pero no usar
    // un salto a continuacion, sino una instruccion CMOVcc
    // (copia condicional).
    // ...

    call kmain

.globl halt  
halt:
    hlt
    jmp halt

.data
.p2align 12
kstack:
    .space KSTACK_SIZE
kstack_end:

