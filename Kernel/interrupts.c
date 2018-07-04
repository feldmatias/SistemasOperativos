
#include "decls.h"
#include "interrupts.h"

#define IDT_DESCRIPTORS 256

static struct IDTR idtr;
static struct Gate idt[IDT_DESCRIPTORS];


// Multiboot siempre define "8" como el segmento de codigo.
// (Ver campo CS en `info registers` de QEMU.)
static const uint8_t KSEG_CODE = 8;

// Identificador de "Interrupt gate de 32 bits" (ver IA32-3A,
// tabla 6-2: IDT Gate Descriptors).
static const uint8_t STS_IG32 = 0xE;


#define outb(port, data) \
        asm("outb %b0,%w1" : : "a"(data), "d"(port));

static void irq_remap() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}


void idt_init(){

	idt_install(T_BRKPT, breakpoint);
	idt_install(T_DIVIDE, divzero);

	idtr.base = (uintptr_t) idt;
	idtr.limit = IDT_DESCRIPTORS * 8 - 1;

	asm("lidt %0" : : "m"(idtr)); //activar el uso de la IDT configurada
}


void irq_init() {
    irq_remap();

    idt_install(T_TIMER, timer_asm);
    idt_install(T_KEYBOARD, keyboard_asm);

    asm("sti");
}


void idt_install(uint8_t n, void (*handler)(void)){
	uintptr_t addr = (uintptr_t) handler;

    idt[n].rpl = 0;
    idt[n].type = STS_IG32;
    idt[n].segment = KSEG_CODE;

    idt[n].off_15_0 = addr & 0xFFFF;
    idt[n].off_31_16 = addr >> 16;

    idt[n].present = 1;
}
