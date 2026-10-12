global  ft_strcmp

section .text

ft_strcmp:
	mov rax, 0
	jmp loop

loop:
	cmp BYTE [rdi + rax], 0
	je exit
	cmp BYTE [rsi + rax], 0
	je exit
	cmp [rdi + rax], [rsi + rax] ; ERROR: need to use a temp registery to move, why ?
	jne exit
	inc rax
	jmp loop

exit:
	sub ; TO COMPLETE
	mov rax, ; TO COMPLETE
	ret


; int	strcmp(const char *s1, const char *s2)
; *s1		-> rdi
; *s2		-> rsi
; return	-> rax