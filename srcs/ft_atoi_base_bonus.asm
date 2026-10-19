; int ft_atoi_base(char *str, char *base)
; *str		-> rdi
; *base		-> rsi
; return	-> rax
global  ft_atoi_base
extern  ft_strlen

section .text
; int isspace(int c)
; c         -> rdi
; return    -> rax
ft_isspace:
    cmp dil, 9     ; '\t'
    je return_true
    cmp dil, 10    ; '\n'
    je return_true
    cmp dil, 11    ; '\v'
    je return_true
    cmp dil, 12    ; '\f'
    je return_true
    cmp dil, 13    ; '\r'
    je return_true
    cmp dil, 32    ; ' '
    je return_true
    jmp return_false

; int ft_issign(int c)
; c         -> rdi
; return    -> rax
ft_issign:
    cmp dil, 45    ; '-'   
    je return_true
    cmp dil, 43    ; '+'
    je return_true
    jmp return_false

; int isdigit(int c)
; c         -> rdi
; return    -> rax
ft_isdigit:
    cmp BYTE [rdi], 48  ; 0
    jge .ft_isdigit2    ; jump if greater or equal (maybe use jae (unsigned version) ?)
    jmp return_false

.ft_isdigit2:
    cmp BYTE [rdi], 57  ; 9
    jle return_true
    jmp return_false

return_true:
    mov rax, 1
    ret

return_false:
    mov rax, 0
    ret


ft_atoi_base:
    push rbx            ; save *str on the stack
    push r12
    mov r12, rdi        ; r12 = *str
    mov rbx, rsi        ; rbx = *base
    xor rcx, rcx
    jmp .ft_valid_base

.ft_valid_base:
    mov rdi, rbx
    call ft_strlen
    cmp rax, 2
    jl .error_exit
    jmp .check_loop

.check_loop:
    cmp BYTE [rbx + rcx], 0
    je .exit
    ; je .check_duplicate; SUITE DU PROGRAMME
    mov dil, BYTE [rbx + rcx]
    call ft_issign
    cmp rax, 1
    je .error_exit
    call ft_isspace
    cmp rax, 1
    je .error_exit
    inc rcx
    jmp .check_loop


.error_exit:
    mov rax, 0
    jmp .end

.exit:
    mov rax, 1
    jmp .end
    
.end:
    pop r12
    pop rbx
    ret

; .ft_check_duplicate:
;     xor rax, rax
;     cmp BYTE [rsi + rcx], 0
;     je                      ; EXIT of the loop, continue here
;     mov r8b, BYTE [rsi + rcx]
;     jmp .ft_check_duplicate_loop
;     inc rcx
;     jmp .ft_check_duplicate

; .ft_check_duplicate_loop:
;     cmp BYTE [rsi + rax], 0
;     je .ft_check_duplicate
;     cmp BYTE [rsi + rax], r8b
;     je .error_exit
;     inc rax
;     jmp .ft_check_duplicate_loop
