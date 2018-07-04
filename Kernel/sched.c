#include "decls.h"
#include "sched.h"

#define MAX_TASK 10
#define IF 0x200
#define STACK_SIZE 4096 

static struct Task tasks[MAX_TASK];
static struct Task *current;
static bool first_iteration = true;

void sched_init() {
    for (size_t i = 0; i < MAX_TASK; i++){
    	if (tasks[i].status == READY){
    		current = &tasks[i];
    		current->status = RUNNING;
    		return;
    	}
    }
}

void spawn(void (*entry)(void)) {
	struct Task* new_task = NULL;
    for (size_t i = 0; i < MAX_TASK; i++){
    	if (tasks[i].status == FREE){
    		new_task = &tasks[i];
    		break;
    	}
    }

    new_task->status = READY;

    uint8_t* stack = &new_task->stack[STACK_SIZE] - sizeof(struct TaskFrame);
    new_task->frame = (struct TaskFrame *)stack;

    new_task->frame->edi = 0;
    new_task->frame->esi = 0;
    new_task->frame->ebp = 0;
    new_task->frame->esp = 0;
    new_task->frame->ebx = 0;
    new_task->frame->edx = 0;
    new_task->frame->ecx = 0;
    new_task->frame->eax = 0;

    new_task->frame->cs = 8;
    new_task->frame->eip = (uint32_t)entry;
    new_task->frame->eflags = IF;
}

void sched(struct TaskFrame *tf) {
    struct Task *new = NULL;
	struct Task *old = current;
	size_t i;

	for (i = 0; i < MAX_TASK; i++){
    	if (&tasks[i] == old){
    		break;
    	}
    }

    old->status = READY;
    while (!new){
    	i++;
    	if (i >= MAX_TASK){
    		i = 0;
    	}
    	if (tasks[i].status == READY){
    		new = &tasks[i];
    		if (first_iteration){
    			first_iteration = false;
    		} else {
    			old->frame = tf;
    		}
    		current = new;
    		current->status = RUNNING;

    		asm("movl %0, %%esp\n"
    			"popa\n"
			    "iret\n"
			    :
			    : "g"(current->frame)
			    : "memory");
    	}
    }

}

void kill_task(){
	current->status = DYING;
    halt();
}