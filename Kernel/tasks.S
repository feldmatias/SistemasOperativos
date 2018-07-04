
.text
.align 4
.globl task_exec
task_exec:
    // Preambulo estandar
    push %ebp
    movl %esp, %ebp

    movl 8(%ebp), %ecx //entry
    movl 12(%ebp), %eax //stack

    leal 0(%eax), %esp
    call *%ecx

    // Restaurar registros callee-saved, si se usaron.
    movl %ebp, %esp
    popl %ebp

    ret



.globl task_swap
task_swap:
    push %ebp
    push %ebx
    push %edi
    push %esi

    movl 20(%esp), %eax //eax = puntero a esp siguiente


    movl %esp, %ecx
    movl 0(%eax), %esp
    movl %ecx, 0(%eax)
        
    pop %esi
    pop %edi
    pop %ebx
    pop %ebp

    ret
