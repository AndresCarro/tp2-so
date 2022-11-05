global read
global write
global exec
global exit
global getpid
global waitpid
global yield
global block
global unblock
global kill
global nice
global malloc
global free
global sem_open
global sem_close
global sem_wait
global sem_post
global pipe
global dup2
global close
global mem_info
global pipe_info
global sem_info
global process_info
global time

section .text

read:
    enter 0,0
    
    mov rax, 0
    int 80h

    leave
    ret

write:
    enter 0,0
    
    mov rax, 1
    int 80h

    leave
    ret

exec:
    enter 0,0
    
    mov rax, 2
    int 80h
    
    leave
    ret

exit:
    enter 0,0
    
    mov rax, 3
    int 80h

    leave
    ret

getpid:
    enter 0,0

    mov rax, 4
    int 80h

    leave
    ret

waitpid:
    enter 0,0

    mov rax, 5
    int 80h

    leave
    ret

yield:
    enter 0,0
    
    mov rax, 6
    int 80h

    leave
    ret

block:
    enter 0,0
    
    mov rax, 7
    int 80h

    leave
    ret

unblock:
    enter 0,0
    
    mov rax, 8
    int 80h

    leave
    ret

kill:
    enter 0,0
    
    mov rax, 9
    int 80h

    leave
    ret

nice:
    enter 0,0

    mov rax, 10
    int 80h

    leave
    ret

malloc:
    enter 0,0
    
    mov rax, 11
    int 80h

    leave
    ret

free:
    enter 0,0
    
    mov rax, 12
    int 80h

    leave
    ret

sem_open:
    enter 0,0
    
    mov rax, 13
    int 80h

    leave
    ret

sem_close:
    enter 0,0
    
    mov rax, 14
    int 80h

    leave
    ret

sem_wait:
    enter 0,0
    
    mov rax, 15
    int 80h

    leave
    ret

sem_post:
    enter 0,0
    
    mov rax, 16
    int 80h

    leave
    ret

pipe:
    enter 0,0
    
    mov rax, 17
    int 80h

    leave
    ret

dup2:
    enter 0,0
    
    mov rax, 18
    int 80h

    leave
    ret

close:
    enter 0,0
    
    mov rax, 19
    int 80h

    leave
    ret

mem_info:
    enter 0,0
    
    mov rax, 20
    int 80h

    leave
    ret

pipe_info:
    enter 0,0
    
    mov rax, 21
    int 80h

    leave
    ret

sem_info:
    enter 0,0
    
    mov rax, 22
    int 80h

    leave
    ret

process_info:
    enter 0,0
    
    mov rax, 23
    int 80h

    leave
    ret

time:
    enter 0,0
    
    mov rax, 24
    int 80h

    leave
    ret