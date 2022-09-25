GLOBAL _defaultExit

section .text

_defaultExit:
    mov rdi, rax
    mov rax, 4
    int 80h
