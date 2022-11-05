global _cli
global _sti
global _master_pic_mask
global _slave_pic_mask
global _hlt

global _irq00_handler
global _irq01_handler
global _irq02_handler
global _irq03_handler
global _irq04_handler
global _irq05_handler

extern irq_dispatcher
extern context_switch

section .text

%macro push_state 0
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
%endmacro

%macro pop_state 0
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
	pop rax
%endmacro

%macro irq_handler_master 1
	push_state

	mov rdi, %1 ; pasaje de parametro
	call irq_dispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	pop_state
	iretq
%endmacro

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

_master_pic_mask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

_slave_pic_mask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out 0A1h, al
    pop rbp
    retn


;8254 Timer (Timer Tick)
_irq00_handler:
	push_state

	mov rdi, 0
	call irq_dispatcher

	mov rdi, rsp
    call context_switch
    mov rsp, rax

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	pop_state
	iretq

;Keyboard
_irq01_handler:
	irq_handler_master 1

;Cascade pic never called
_irq02_handler:
	irq_handler_master 2

;Serial Port 2 and 4
_irq03_handler:
	irq_handler_master 3

;Serial Port 1 and 3
_irq04_handler:
	irq_handler_master 4

;USB
_irq05_handler:
	irq_handler_master 5