global cpu_vendor

global read_port

global get_seconds
global get_minutes
global get_hours
global get_day
global get_month
global get_year

section .text
	
cpu_vendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

read_port:
    push rbp
    mov rbp, rsp
    
    mov dx, di
    in al, dx

    mov rsp, rbp
    pop rbp 
    ret

set_binary_bit_rtc:
	enter 0,0

	mov al, 0Bh
	out 70h, al
	in al, 71h
	or al, 0b00000100
	out 71h, al

	leave
	ret

get_seconds:
    enter 0,0
    
	call set_binary_bit_rtc
    mov al, 0
    out 70h, al
    in al, 71h

    leave
    ret

get_minutes:
    enter 0,0

	call set_binary_bit_rtc
    mov al, 2
    out 70h, al
    in al,71h

    leave
    ret

get_hours:
    enter 0,0

	call set_binary_bit_rtc
    mov al, 4
    out 70h, al
    in al, 71h

    leave
    ret	

get_day:
    enter 0,0

	call set_binary_bit_rtc
    mov al, 7
    out 70h, al
    in al, 71h

    leave
    ret		

get_month:
    enter 0,0

	call set_binary_bit_rtc
    mov al, 8
    out 70h, al
    in al, 71h

    leave
    ret	

get_year:
	enter 0,0

	call set_binary_bit_rtc
    mov al, 9
    out 70h, al
    in al, 71h

    leave
    ret