global load_process
global _int20h

extern context_switch

section .text

_start:
    call rcx
    mov rdi, rax
    mov rax, 3
    int 80h

; rdi -> rip
; rsi -> rsp
; rdx -> argc
; rcx -> argv
load_process:
    enter 0, 0

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

_int20h:
    int 20h
    ret