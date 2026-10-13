global  ft_strcmp

section .text

ft_strcmp:
	mov rax, 0
	jmp loop

loop:
	cmp BYTE [rdi + rax], 0			; if s1[0] == \0, exit
	je exit
	cmp BYTE [rsi + rax], 0			; if s2[0] == \0, exit
	je exit
	mov r10, BYTE [rsi + rax]		; mov [rsi + rax] in a temp registery to compare
	cmp BYTE [rdi + rax], r10 		; need to use a temp registery to move, why ?
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