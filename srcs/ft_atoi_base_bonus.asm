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

ft_check_duplicate:
    xor rax, rax
    xor rcx, rcx
    jmp .outer_loop

.outer_loop:
    cmp BYTE [rdi + rcx], 0 ; if base[i] = '\0', all string have been checked without founding duplicate
    je return_false         ; -> return false (0)
    lea rdx, [rcx + 1]      ; load effective address, j = i + 1

.inner_loop:
    cmp BYTE [rdi + rdx], 0     ; if if base[j] = '\0', base[i] have been checked
    je .next                    ; -> go to .next to check the next character
    mov r8b, BYTE [rdi + rdx]   ; move base[j] in r8b
    cmp BYTE [rdi + rcx], r8b   ; cmp base[i] with base[j]
    je return_true              ; if equal, return true (1)
    inc rdx                     ; j++
    jmp .inner_loop             ; go at the begining of the loop 

.next:
    inc rcx                     ; i++
    jmp .outer_loop             ; go at the begining of the outer loop

return_true:
    mov rax, 1
    ret

return_false:
    mov rax, 0
    ret


ft_atoi_base:
    push rbx            ; save rbx on the stack
    push r12            ; save r12 on the stack
    mov r12, rdi        ; r12 = *str
    mov rbx, rsi        ; rbx = *base
    xor rcx, rcx        ; initialise rcx at 0

.ft_valid_base:
    mov rdi, rbx        ; move *base in rdi (first arg)
    call ft_strlen      ; get the lenght of *base
    cmp rax, 2          
    jl .error_exit      ; if ft_strlen(*base) < 2, jump to error_exit
    jmp .check_loop     ; else, continue to check_loop

.check_loop:
    cmp BYTE [rbx + rcx], 0     ; cmp base[i] and '\0' 
    ; je .exit                    
    je .check_duplicate       ; if equal, all *base have been checked successufully, exit the loop and continue
    mov dil, BYTE [rbx + rcx]   ; move base[i] in rdi 8bit (dil)
    call ft_issign              ; check if '+' or '-' are present
    cmp rax, 1                  ; cmp the return value 
    je .error_exit              ; if equal (true), go to error_exit 
    call ft_isspace             ; check if whitespace are present 
    cmp rax, 1                  ; cmp the return value
    je .error_exit              ; if equal (true), go to error_exit
    inc rcx                     ; increment i 
    jmp .check_loop             ; return at the begining of the loop

.check_duplicate:
    mov rdi, rbx
    call ft_check_duplicate
    cmp rax, 1
    je .error_exit
    jmp .exit
    ; jmp .convert

.convert:
    xor rcx, rcx                ; i = 0

.skip_whitspace:
    cmp BYTE [r12 + rcx], 0     ; if str[i] = '\0', return 0
    je .error_exit
    mov dil, BYTE [r12 + rcx]   ; move str[i] in rdi 8bit (dil)
    call ft_isspace
    cmp rax, 0                  ; if isspace(str[i]) == False, go to next step
    je .exit        
    inc rcx                     ; i++
    jmp .skip_whitspace         ; continue to check next char
    ; adapt the sign if sign present (did i need to handle multiple sign ?)
    ; while str[i] is a base character, make the conversion

.error_exit:
    mov rax, 0                  ; set return value to 0
    jmp .end

.exit:
    mov rax, 1
    jmp .end
    
.end:
    pop r12                     
    pop rbx
    ret

