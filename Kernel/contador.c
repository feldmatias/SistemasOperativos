#include "decls.h"
#include "sched.h"

#define COUNTLEN 20
#define TICKS (1ULL << 15)
#define DELAY(x) (TICKS << (x))
#define USTACK_SIZE 1024 

static volatile char *const VGABUF = (volatile void *) 0xb8000;

static uintptr_t esp;
static uint32_t stack1[USTACK_SIZE] __attribute__((aligned(4096)));
static uint32_t stack2[USTACK_SIZE] __attribute__((aligned(4096)));

static void yield() {
    if (esp)
        task_swap(&esp);
}

static void exit() {
    uintptr_t tmp = esp;
    esp = 0;
    task_swap(&tmp);
}

static void contador(unsigned lim, uint8_t linea, char color, bool do_yield){
    char counter[COUNTLEN] = {'0'};  // ASCII digit counter (RTL).

    while (lim--) {
        char *c = &counter[COUNTLEN];
        volatile char *buf = VGABUF + 160 * linea + 2 * (80 - COUNTLEN);

        unsigned p = 0;
        unsigned long long i = 0;

        while (i++ < DELAY(6))  // Usar un entero menor si va demasiado lento.
            ;

        while (counter[p] == '9') {
            counter[p++] = '0';
        }

        if (!counter[p]++) {
            counter[p] = '1';
        }

        while (c-- > counter) {
            *buf++ = *c;
            *buf++ = color;
        }

        if (do_yield){
            yield();
        }
    }
}

void contador_round_robin(unsigned lim, uint8_t linea, char color) {
    contador (lim, linea, color, false);
    kill_task();
}

static void contador_yield(unsigned lim, uint8_t linea, char color) {
    contador (lim, linea, color, true);
}


void contador_run() {

    uintptr_t *a = stack1 + sizeof(stack1);
    uintptr_t *b = stack2 + sizeof(stack2);

    *(--a) = 0x2F;  //Color
    *(--a) = 0;     //Linea
    *(--a) = 100;   //Limite

    *(--b) = 0x4F;  //Color
    *(--b) = 1;     //Linea
    *(--b) = 90;    //Limite

    //Direccion de retorno de la funcion contador_yield
    *(--b) = (uintptr_t)exit;

    //Direccion de retorno de la funcion task_swap en la primer iteracion
    *(--b) = (uintptr_t)contador_yield; 
    
    //Registros calle-saved (ebp, ebx, esi, edi)
    *(--b) = 0;
    *(--b) = 0;
    *(--b) = 0;
    *(--b) = 0;

    esp = (uintptr_t)b;

    task_exec((uintptr_t) contador_yield, (uintptr_t) a);
}
