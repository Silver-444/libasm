global  ft_read
extern __errno_location

section .text

ft_read: 
        mov rax, 0	; 0 = read syscall number
        syscall
		js error

error: 
        mov rax, -1
		ret

; ssize_t	read(int fd, void *buf, size_t count)
; fd		-> rdi
; *buf		-> rsi
; count		-> rdx
; return	-> rax
