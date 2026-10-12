global  ft_strcpy

section .text

ft_strcpy:
	mov rax, 0
	jmp loop

loop:
	mov [rdi + rax], [rsi + rax] ; ERROR: need to use a temp registery to move, why ?
	cmp BYTE [rdi + rax], 0
	je exit
	inc rax
	jmp loop

exit:
	mov rax, rdi
	ret

; char	*strcpy(char *dst, const char *src)
; *dst		-> rdi
; *src		-> rsi
; return	-> rax