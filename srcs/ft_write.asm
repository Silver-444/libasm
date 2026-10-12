global  ft_write
extern __errno_location

section .text

ft_write: 
	mov rax, 1	; 1 = write syscall number
	syscall
	js error

error: 
	; call __errno_location wrt ..plt
    mov rax, -1
	ret


; ssize_t	write(int fd, const void *buf, size_t count)
; fd		-> rdi
; *buf		-> rsi
; count		-> rdx
; return	-> rax
