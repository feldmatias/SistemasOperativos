#include "decls.h"
#include "sched.h"

#define MAX 1500


static void contador1() {
    contador_round_robin(MAX, 2, (char)0xB0);
}

static void contador2() {
    contador_round_robin(MAX, 3, (char)0xD0);
}

static void contador3() {
    contador_round_robin(MAX, 4, (char)0xE0);
}

void contador_spawn() {
    spawn(contador1);
    spawn(contador2);
    spawn(contador3);
}

void lab_kernel(){

	contador_run();

    asm("int3");


    int8_t linea;
    uint8_t color;

    asm("div %4"
        : "=a"(linea), "=c"(color)
        : "0"(18), "1"(0xE0), "b"(0), "d"(0));

    vga_write2("Funciona vga_write2?", linea, color);

    kill_task();
}

void kmain(const multiboot_info_t *mbi) {
    vga_write("kern2 loading.............", 8, 0x70);

    if (mbi && mbi->flags) {
        char buf[256] = "cmdline: ";
        char *cmdline = (void *) mbi->cmdline;

        strlcat (buf, cmdline, sizeof(buf));
        vga_write(buf, 9, 0x07);

        print_mbinfo(mbi);
    }

    two_stacks();
    two_stacks_c();

    //Ejercicios antes del promocional
    spawn(lab_kernel);

    //Ejercicio promocional
    contador_spawn();
    sched_init();

    idt_init();
    irq_init();
}
