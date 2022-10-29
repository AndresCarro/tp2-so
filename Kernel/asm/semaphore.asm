GLOBAL spinlock
GLOBAL unlock

spinlock:
    mov rax, 0
    mov al, 1
  
    xchg al, [rdi]
    cmp al, 0
    jne spinlock

    ret

unlock:
    mov byte [rdi], 0
    ret