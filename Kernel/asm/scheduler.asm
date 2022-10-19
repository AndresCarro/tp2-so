GLOBAL _defaultExit

section .text

_defaultExit:
    mov rdi, rax
    mov rax, 4
    int 80h


scheduler_handler:
    pushState
    
    mov rdi, rsp
    call context_switch
    mov rsp, rax

    mov al, 20h
    out 20h, al
    
    popState

    iretq

_start:
    ; Como en RCX quedo el entryPoint que estaba en rdi entonces
    ; debo llamar a este registro
    call rcx
    ; La función de userland tiene su entrypoint en rcx y va a devolver algo
    ; ese algo se guarda en rax.
    ; Guardamos el return value de la funcion en rdi para pasarselo como primer
    ; parametro a la syscall
    mov rdi, rax
    ; TODO mov rax, Numero de syscall de exit
    mov rax, 
    int 80h

; Creamos el stack "simulado" del proceso para que el scheduler
; pueda tomar el programa y correrlo
; rdi -> entryPoint, el puntero a funcion rip
; rsi -> rsp
; rdx -> argc
; rcx -> argv
load_process:
    enter 0, 0

    ; Muevo el rsp que me pasan por parametro a el registro rsp
    mov rsp, rsi

    push qword 0x0      ; SS
    push rsi            ; RSP
    push qword 0x202    ; RFLAGS
    push qword 0x8      ; CS
    push _start         ; RIP

    push qword 0x0      ; RAX
    push qword 0x0      ; RBX
    push rdi            ; RCX
    push qword 0x0      ; RDX
    push qword 0x0      ; RBP
    push rdx            ; RDI
    push rcx            ; RSI
    push qword 0x0      ; R8
    push qword 0x0      ; R9
    push qword 0x0      ; R10
    push qword 0x0      ; R11
    push qword 0x0      ; R12
    push qword 0x0      ; R13
    push qword 0x0      ; R14
    push qword 0x0      ; R15

    mov rax, rsp

    leave
    ret    	