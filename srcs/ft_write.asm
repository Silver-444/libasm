global  ft_write
extern __errno_location

section .text

ft_write: 
	mov rax, 1	; 1 = write syscall number
	syscall		; call system function
	cmp rax, 0	; compare the return value to 0
	jl error	; if the result is less than 0, jump to error
	ret

error:
	neg rax							; get absolute value of the return value
	mov rdi, rax					; stock return value of write because rax will be filled with errno_location return value
	call __errno_location wrt ..plt	; call errno_location function to get the adress of errno variable
	mov [rax], rdi					; set the value of errno (stock in rdi) (rax contain the adress of errno)
    mov rax, -1						; set the return value to -1
	ret


; ssize_t	write(int fd, const void *buf, size_t count)
; fd		-> rdi
; *buf		-> rsi
; count		-> rdx
; return	-> rax
