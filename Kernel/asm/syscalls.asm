GLOBAL _syscallHandler
EXTERN syscallDispatcher

SECTION .text

_syscallHandler:
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15

    ;rax, rdi, rsi, rdx |||, r10, r8, r9

	;rdi, rsi, rdx, rcx |||, r8 y r9
	mov rcx, rax
	mov r8, rsp
    call syscallDispatcher

	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	; Instead of pop rax
	add rsp, 8

    iretq