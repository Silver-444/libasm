; int ft_atoi_base(char *str, char *base)
; *str		-> rdi
; *base		-> rsi
; return	-> rax
global  ft_atoi_base
extern  ft_strlen

section .text
; int isspace(int c)
; c         -> dil
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
; c         -> dil
; return    -> rax
ft_issign:
    cmp dil, 45    ; '-'   
    je return_true
    cmp dil, 43    ; '+'
    je return_true
    jmp return_false


; int ft_inbase(int c)
; c         -> dil
; base      -> rsi
; return    -> rax (c index in base or -1 if not found)
ft_inbase:
    xor rax, rax                ; i = 0

.loop:
    cmp BYTE [rsi + rax], 0     ; if base[i] == '\0', c not found in base
    je .not_found
    cmp BYTE [rsi + rax], dil   ; if base[i] == c, c found in base
    je .found
    inc rax                     ; i++
    jmp .loop

.not_found:
    mov rax, -1
    ret

.found:
    ret                         ; return rax = i


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
    push r13            ; save r13 on the stack
    push r14
    push r15
    mov r12, rdi        ; r12 = *str
    mov rbx, rsi        ; rbx = *base
    xor r14, r14        ; initialise r14 at 0

;;;;;;;;;;;;;; Checking if base is valid ;;;;;;;;;;;;;
    mov rdi, rbx        ; move *base in rdi (first arg)
    call ft_strlen      ; get the lenght of *base
    cmp rax, 2          
    jl .error_exit      ; if ft_strlen(*base) < 2, jump to error_exit
    mov r13, rax        ; r13 = ft_strlen(*base)
    jmp .check_loop     ; else, continue to check_loop

.check_loop:
    movzx edi, BYTE [rbx, r14]
    cmp dil, 0                  ; cmp base[i] and '\0'                  
    je .check_duplicate         ; if equal, all *base have been checked successufully, exit the loop and continue
    call ft_issign              ; check if '+' or '-' are present
    cmp rax, 1                  ; cmp the return value 
    je .error_exit              ; if equal (true), go to error_exit
    movzx edi, BYTE [rbx, r14]
    call ft_isspace             ; check if whitespace are present 
    cmp rax, 1                  ; cmp the return value
    je .error_exit              ; if equal (true), go to error_exit
    inc r14                     ; increment i 
    jmp .check_loop             ; return at the begining of the loop

.check_duplicate:
    mov rdi, rbx
    call ft_check_duplicate
    cmp rax, 1
    je .error_exit
    jmp .skip_space

;;;;;;;;;;;;;; Skip whitespace at the begining of str ;;;;;;;;;;;;;
.skip_space:
    movzx edi, BYTE [r12]
    call ft_isspace
    cmp rax, 0                  ; if isspace(str[i]) == False, go to next step
    je .sign_init        
    inc r12                     ; i++
    jmp .skip_space             ; continue to check next char

;;;;;;;;;;;;;; Set sign ;;;;;;;;;;;;;
.sign_init:
    mov r14, 1                  ; r14 = sign (initialise with 1)
    movzx edi, BYTE [r12]
    call ft_issign
    cmp rax, 0
    je .convert_init
    cmp BYTE [r12], 45          ; '-'
    je .set_negative
    inc r12
    jmp .convert_init

.set_negative:
    neg r14                 ; 1 -> -1
    inc r12
    jmp .convert_init

;;;;;;;;;;;;;; Conversion ;;;;;;;;;;;;;
.convert_init:
    xor r15, r15                ; nbr = 0

.convert_loop:
    movzx edi, BYTE [r12]       ; c = *str
    mov rsi, rbx                ; rsi = base
    call ft_inbase
    cmp rax, 0                  ; if c not in base, return -1
    jl .done
    imul r15, r13               ; nbr *= len(base) (imul = signed integer multiplication)
    add r15, rax                ; nbr = nbr + index in base
    inc r12
    jmp .convert_loop



.error_exit:
    xor rax, rax                  ; set return value to 0
    jmp .end

.done:
    imul r15, r14   ; nbr *= sign
    mov rax, r15
    jmp .end
    
.end:
    pop r15
    pop r14
    pop r13
    pop r12                     
    pop rbx
    ret

