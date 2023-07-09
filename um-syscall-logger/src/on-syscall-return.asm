section .data
use64

global asm_block_start
asm_block_start:
	dq 0

global host_handle_value
host_handle_value:
	dq 0

global host_callback_address
host_callback_address:
	dq 0

global NtCreateThreadEx
NtCreateThreadEx:
	dq 0

global NtAllocateVirtualMemory
NtAllocateVirtualMemory:
	dq 0

global instrumentation_callback_stub
instrumentation_callback_stub:
	cmp qword [rsp], 0 ; ntdll.dll!KiUserCallbackDispatcher
	je .jmp

.prologue:
	push rcx
	push rdx
	push r8
	push r9

	lea rcx, [rel asm_block_start]
	lea rdx, [rel asm_block_end]

	cmp rcx, [rsp + 32] ; [rsp] = return address
	ja .start ; rcx > [rsp] ? jmp out
	
	cmp rdx, [rsp + 32]
	ja .epilogue ; rdx > [rsp] ? jmp to end

.start:
	push rax ; save return value
	push r10 ; save return address

	sub rsp, 96 ; reserve stack space

.allocate:
	mov qword [rsp + 40], 0x04 ; PAGE_READWRITE

	mov qword [rsp + 32], 0x3000 ; MEM_RESERVE | MEM_COMMIT
	and dword [rsp + 32], 0xFFFFFFC0 ; (https://cdn.discordapp.com/attachments/765576637265739789/1127338721369403574/ida64_TdfdkFjwRl.png)

	lea r9, [rsp + 88]
	mov qword [r9], 24 ; move size of call_info_t struct
	
	xor r8, r8 ; ZeroBits

	lea rdx, [rsp + 80] ; BaseAddress
	mov qword [rdx], 0 ; make sure its 0

	mov rcx, -1 ; ProcessHandle

	lea rax, [rel NtAllocateVirtualMemory]
	call [rax] ; call

.populate:
	mov rcx, [rsp + 80] ; BaseAddress

	mov rdx, [rsp + 144]
	mov qword [rcx], rdx ; set return address

	mov rax, [rsp + 104] ; read return value
	mov dword [rcx + 8], eax ; set return value

	mov r10, [rsp + 96] ; r10 has been modified by NtAllocateVirtualMemory, so we restore it

	; inlined the loop nvm
.loopstart:
	cmp byte [r10], 0xB8
	je .found

	; didnt match
	dec r10
	jmp .loopstart
	
.found:
	mov eax, [r10 + 1] ; found it

	mov dword [rcx + 12], eax ; set syscall id

	mov rax, gs:0x30 ; read TEB
	mov rdx, [rax + 0x48] ; read ->ClientId.UniqueThread (https://cdn.discordapp.com/attachments/765576637265739789/1127179188038811648/ida64_FuXhzSdOJe.png)

	mov dword [rcx + 16], edx ; set thread id

	mov rdx, [rax + 0x40] ; read ->ClientId.UniqueProcess (https://cdn.discordapp.com/attachments/765576637265739789/1127259074694746212/ida64_yTUzSN38jQ.png)

	mov dword [rcx + 20], edx ; set process id

.dispatch:
	; memory is done at this point, we can pass it to host

	mov qword [rsp + 80], 0 ; extended attributes (?)
	mov qword [rsp + 72], 0 ; stack reserve size
	mov qword [rsp + 64], 0 ; stacksize
	mov qword [rsp + 56], 0 ; ?
	mov qword [rsp + 48], 0 ; CREATE_SUSPENDED (0)
	mov qword [rsp + 40], rcx ; rcx contains address of memory

	lea r9, [rel host_callback_address] ; read relative
	mov r9, [r9]
	mov qword [rsp + 32], r9 ; callback address

	lea r9, [rel host_handle_value] ; read relative
	mov r9, [r9] ; process handle

	xor r8, r8 ; object attributes
	mov rdx, 0x1FFFFF ; access parameter
	lea rcx, [rsp + 88] ; thread handle ref

	lea rax, [rel NtCreateThreadEx] ; read relative
	call [rax] ; do the call

.end:
	add rsp, 96 ; reclaim stack space

	pop r10 ; restore return address
	pop rax ; restore return value

.epilogue:
	; restore used
	pop r9
	pop r8
	pop rdx
	pop rcx

.jmp:
	; jmp to return address
	jmp r10

global asm_block_end
asm_block_end:
	dq 0

