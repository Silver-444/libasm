global	ft_strlen

section .text

ft_strlen: 
	xor rax, rax			; set rax to 0 (optimise compare to mov rax, 0)

.loop:   
	cmp BYTE [rdi + rax], 0	; compare byte value with 0
	je  .exit               ; jump to exit if cmp equal
	inc rax                 ; increment rax
	jmp .loop               ; jump at the begining of loop

.exit: 
	ret

; size_t	strlen(const char *str)
; *str		-> rdi
; return	-> rax
