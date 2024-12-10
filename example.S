	section .text
	global _start
_start:
	call   main
	mov    rdi, rax
	mov    rax, 60
	syscall
main:
	push rbp
	mov rbp, rsp
	pop rbp
	mov eax, 0
	neg eax
	not eax
	ret
