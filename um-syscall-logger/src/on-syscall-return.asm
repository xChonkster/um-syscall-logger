section .data
use64

global ASM_BLOCK_START
ASM_BLOCK_START:
	dq 0

global HOST_HANDLE_VALUE
HOST_HANDLE_VALUE:
	dq 0

global HOST_CALLBACK_ADDRESS:
HOST_CALLBACK_ADDRESS:
	dq 0

global NtCreateThreadEx
NtCreateThreadEx:
	dq 0

global NtWaitForSingleObject
NtWaitForSingleObject:
	dq 0

global ON_SYSCALL_RETURN
ON_SYSCALL_RETURN:
	; int 3 ; - software interrupt to invoke the debugger

	; check for recursion

	lea rcx, [rel ASM_BLOCK_START] ; start of asm block
	cmp rcx, [rsp] ; top of stack = return address
	ja .START ; jump out if less than &ASM_BLOCK_START
	
	lea rcx, [rel ASM_BLOCK_END] ; end of asm block
	cmp rcx, [rsp]
	ja .END ; if (retaddr > &ASM_BLOCK_START && retaddr < &ASM_BLOCK_END) return;

	; https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention?view=msvc-170#callercallee-saved-registers

.START:
	push rax ; savesyscall return value
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

	sub rsp, 112 ; reserve space for handle, args, struct and 32 bytes scratch space

	mov [rsp + 96], rax ; save return address in stack space

	mov rax, gs:0x30 ; read TEB
	mov ecx, [rax + 0x48] ; read ->ClientId.UniqueThread (https://cdn.discordapp.com/attachments/765576637265739789/1127179188038811648/ida64_FuXhzSdOJe.png)
	mov edx, [rax + 0x40] ; read ->ClientId.UniqueProcess (https://cdn.discordapp.com/attachments/765576637265739789/1127259074694746212/ida64_yTUzSN38jQ.png)

	mov dword [rsp + 104], ecx ; save thread id
	mov dword [rsp + 108], edx ; save process id

	; look for syscall id at return address
.READLOOP:
	cmp byte [r10], 0xB8 ; mov eax, ...
	je .FOUNDMOVEAX

	cmp byte [r10], 0x48 ; mov rax, ...
	jne .CONTINUE

	cmp byte [r10 + 1], 0xC7
	jne .CONTINUE

	cmp byte [r10 + 2], 0xC0
	je .FOUNDMOVRAX
	
.CONTINUE:
	dec r10 ; dec r10 until we get to "mov eax, ..." or "mov rax, ..." (gg manual syscalls)
	jmp .READLOOP

.FOUNDMOVEAX:
	mov eax, dword [r10 + 1]
	mov dword [rsp + 100], eax ; rsp + 96 = struct + 4 + 4 = 104 = syscall_id
	jmp .CREATETHREAD

.FOUNDMOVRAX: ; untested
	mov eax, dword [r10 + 3]
	mov dword [rsp + 100], eax

.CREATETHREAD:
	; when you do a push, the stack grows down, so the first push is the furthest up

	mov qword [rsp + 80], 0 ; extended attributes (?)
	mov qword [rsp + 72], 0 ; stack reserve size
	mov qword [rsp + 64], 0 ; stacksize
	mov qword [rsp + 56], 0 ; ?
	mov qword [rsp + 48], 0 ; CREATE_SUSPENDED (0)
	lea rax, [rsp + 96] ; get address of struct
	mov qword [rsp + 40], rax ; parameter

	lea r9, [rel HOST_CALLBACK_ADDRESS] ; read relative
	mov r9, [r9]
	mov qword [rsp + 32], r9 ; callback address
	
	lea r9, [rel HOST_HANDLE_VALUE] ; read relative
	mov r9, [r9] ; process handle

	mov r8, 0 ; object attributes
	mov rdx, 0x1FFFFF ; access parameter
	lea rcx, [rsp + 88] ; thread handle ref

	lea rax, [rel NtCreateThreadEx] ; read relative
	call [rax] ; do the call

	; we now need to NtWaitForSingleObject

	mov r8, 0 ; Timeout (0 = INFINITE) (https://cdn.discordapp.com/attachments/765576637265739789/1127251571491749978/ida64_h9aWTslnk6.png)
	mov rdx, 0 ; Alertable
	mov rcx, [rsp + 88] ; handle value

	lea rax, [rel NtWaitForSingleObject] ; read relative
	call [rax] ; do the call:tm:

	; reclaim stack space
	add rsp, 112 ; 8 (handle) + 32 (scratch) + (7 * 8 = 56) (args) + (4 * 4 = 16) (struct) = 112
	
	; restore registers
	pop r15
	pop r14
	pop r13
	pop r12
	; pop rsp
	pop rsi
	pop rdi
	pop rbp
	pop rbx

	pop r10 ; restore syscall return address
	pop rax ; restore syscall return value

.END:
	jmp r10

global ASM_BLOCK_END
ASM_BLOCK_END:
	dq 0

