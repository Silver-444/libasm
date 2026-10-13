global  ft_read
extern __errno_location

section .text

ft_read: 
        mov rax, 0	; 0 = read syscall number
        syscall
        cmp rax, 0
		jl error
        ret

error: 
	neg rax							; get absolute value of the return value
	mov rdi, rax					; stock return value of write because rax will be filled with errno_location return value
	call __errno_location wrt ..plt	; call errno_location function to get the adress of errno variable
	mov [rax], rdi					; set the value of errno (stock in rdi) (rax contain the adress of errno)
    mov rax, -1						; set the return value to -1
	ret

; ssize_t	read(int fd, void *buf, size_t count)
; fd		-> rdi
; *buf		-> rsi
; count		-> rdx
; return	-> rax
