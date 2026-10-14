global  ft_strcmp

section .text

ft_strcmp:
	xor rax, rax					; set rax to 0

.loop:
	cmp BYTE [rdi + rax], 0			; if s1[i] == '\0', exit
	je .exit
	cmp BYTE [rsi + rax], 0			; if s2[i] == '\0', exit
	je .exit
	mov r8b, BYTE [rsi + rax]		; move s2[i](= [rsi + rax]) in a temp registery to compare (r8b: caller saved, 8-bit) 
	cmp BYTE [rdi + rax], r8b 		; need to use a temp registery because memory to memory is not possible
	jne .exit						; if s1[i] != s2[i], exit
	inc rax							; increment i
	jmp .loop						; jump at the begining of the loop

.exit:
	movzx r8, BYTE [rdi + rax]		; move s1[i] and fill the rest of r8 with 0
	movzx r9, BYTE [rsi + rax]		; move s2[i] and fill the rest of r8 with 0
	sub r8, r9						; s1 - s2
	mov rax, r8						; move s1 - s2 relust in return value rax 
	ret


; int	strcmp(const char *s1, const char *s2)
; *s1		-> rdi
; *s2		-> rsi
; return	-> rax
