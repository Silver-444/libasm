global  ft_strcpy

section .text

ft_strcpy:
	xor rax, rax					; set rax to 0

.loop:
	cmp BYTE [rsi + rax], 0			; check if src[i] is '\0'
	je .exit						; jump to exit if equal
	mov r8b, BYTE [rsi + rax]		; mov src[i](= [rsi + rax]) in a temp registery to compare (r8b: caller saved, 8-bit)
	mov BYTE [rdi + rax], r8b		; need to use a temp registery because memory to memory is not possible
	inc rax							; increment i
	jmp .loop						; jump back at the begining of the loop

.exit:
	mov BYTE [rdi + rax], 0				; add '\0' at the end of dst
	mov rax, rdi					; set return value with *dst
	ret

; char	*strcpy(char *dst, const char *src)
; *dst		-> rdi
; *src		-> rsi
; return	-> rax
