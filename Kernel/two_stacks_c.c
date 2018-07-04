#include "decls.h"
#define USTACK_SIZE 1024

static uint32_t stack1[USTACK_SIZE] __attribute__((aligned(4096)));
static uint32_t stack2[USTACK_SIZE] __attribute__((aligned(4096)));

void two_stacks_c() {
    // Inicializar al *tope* de cada pila.
    uintptr_t *a = stack1 + sizeof(stack1);
    uintptr_t *b = stack2 + sizeof(stack2);

    // Preparar, en stack1, la llamada:
    //vga_write("vga_write() from stack1", 15, 0x57);

    // AYUDA 1: se puede usar alguna forma de pre- o post-
    // incremento/decremento, segun corresponda:
    //
    //     *(a++) = ...
    //     *(++a) = ...
    //     *(a--) = ...
    //     *(--a) = ...
    
    
    *(--a) = 0x57;

    *(--a) = 15;

    *(--a) = (uintptr_t) "vga_write() from stack1";

    // AYUDA 2: para apuntar a la cadena con el mensaje,
    // es suficiente con el siguiente cast:
    //
    //   ... a ... = (uintptr_t) "vga_write() from stack1";

    // Preparar, en s2, la llamada:
    //vga_write("vga_write() from stack2", 16, 0xD0);

    // AYUDA 3: para esta segunda llamada, usar esta forma de
    // asignacion alternativa:
    b -= 3;
    b[0] = (uintptr_t) "vga_write() from stack2";
    b[1] = 16;
    b[2] = 0xD0;

    // Primera llamada usando task_exec().
    task_exec((uintptr_t) vga_write, (uintptr_t) a);

    // Segunda llamada con ASM directo. Importante: no
    // olvidar restaurar el valor de %esp al terminar, y
    // compilar con: -fasm -fno-omit-frame-pointer.
    
    asm("movl %0, %%esp; call *%1; movl %%ebp, %%esp"
        : /* no outputs */
        : "r"(b), "r"(vga_write));
}

