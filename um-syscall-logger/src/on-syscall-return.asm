section .text
use64

extern ON_SYSCALL_RETURN_IMPL

global ON_SYSCALL_RETURN
ON_SYSCALL_RETURN:
	
	; https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention?view=msvc-170#callercallee-saved-registers

	push rax ; save syscall return value
	push r10 ; save syscall return address

	push rbx
	push rbp
	push rdi
	push rsi
	; push rsp
	push r12
	push r13
	push r14
	push r15

	mov rcx, r10 ; move return address
	mov rdx, rax ; move syscall result

	sub rsp, 32 ; reserve stack space
	call ON_SYSCALL_RETURN_IMPL
	add rsp, 32 ; remove stack space

	pop r15
	pop r14
	pop r13
	pop r12
	; pop rsp
	pop rsi
	pop rdi
	pop rbp
	pop rbx

	jmp r10