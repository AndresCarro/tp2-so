GLOBAL sys_write
GLOBAL sys_read
GLOBAL sys_getregs
GLOBAL sys_exit
GLOBAL exec
GLOBAL waitpid
GLOBAL nice
GLOBAL sys_time
GLOBAL sys_copymem
GLOBAL inv_opcode
GLOBAL div_zero
GLOBAL sem_open
GLOBAL sem_close
GLOBAL sem_wait
GLOBAL sem_post
GLOBAL pipe
GLOBAL dup2

GLOBAL halt

section .text
halt:
    hlt
    ret

sys_write:
    push rbp
    mov rbp, rsp
    mov rax, 1
    int 0x80
    mov rsp, rbp
    pop rbp
    ret

sys_read:
    push rbp
    mov rbp, rsp
    mov rax, 0
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

sys_getregs:
    push rbp
    mov rbp, rsp
    mov rax, 2
    int 0x80
    mov rsp, rbp
    pop rbp
    ret

sys_exit:
    push rbp
    mov rbp,rsp
    mov rax,4
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

exec:
    enter 0,0
    
    mov rax, 3
    int 80h
    
    leave
    ret

waitpid:
    enter 0,0

    mov rax, 10
    int 80h

    leave
    ret

nice:
    enter 0,0

    mov rax, 11
    int 80h

    leave
    ret

sys_time:
    push rbp
    mov rbp,rsp
    mov rax,5
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

sys_copymem:
    push rbp
    mov rbp,rsp
    mov rax,6
    int 0x80
    mov rsp,rbp
    pop rbp
    ret

inv_opcode:
    ud2
    ret

div_zero:
    mov ecx, 0
    div ecx
    ret

sem_open:
    enter 0,0

    mov rax, 12
    int 80h

    leave
    ret

sem_close:
    enter 0,0

    mov rax, 13
    int 80h

    leave
    ret

sem_wait:
    enter 0,0

    mov rax, 14
    int 80h

    leave
    ret

sem_post:
    enter 0,0

    mov rax, 15
    int 80h

    leave
    ret

pipe:
    enter 0,0

    mov rax, 16
    int 80h

    leave
    ret

dup2:
    enter 0,0

    mov rax, 17
    int 80h

    leave
    ret