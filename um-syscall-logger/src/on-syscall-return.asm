section .text
use64

extern ON_SYSCALL_RETURN_IMPL

global ON_SYSCALL_RETURN
ON_SYSCALL_RETURN:
	
	; https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention?view=msvc-170#callercallee-saved-registers

	push rax ; save return value of syscall
	push rcx
	push rdx
	push r8
	push r9
	push r10 ; save return address of syscall
	push r11

	mov rcx, r10 ; move return address
	mov rdx, rax ; move syscall result

	sub rsp, 32 ; reserve stack space
	call ON_SYSCALL_RETURN_IMPL
	add rsp, 32 ; remove stack space

	pop r11
	pop r10
	pop r9
	pop r8
	pop rdx
	pop rcx
	pop rax

	jmp r10