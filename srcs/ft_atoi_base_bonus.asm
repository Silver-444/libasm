global  ft_atoi_base
extern  ft_strlen


section .text


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; UTILS FUNCTIONS ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;; isspace fonction return 1 (True if the character c is a whitespace)
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


;;;;; is_sign fonction return 1 (True if the character c is a sign)
; int ft_issign(int c)
; c         -> dil
; return    -> rax
ft_issign:
    cmp dil, 45    ; '-'   
    je return_true
    cmp dil, 43    ; '+'
    je return_true
    jmp return_false


;;;;; inbase fonction return index in the base when found, -1 if not
; int ft_inbase(int c, char *base)
; c         -> dil
; base      -> rsi
; return    -> rax
ft_inbase:
    xor rax, rax                ; i = 0

.loop:
    cmp BYTE [rsi + rax], 0     ; if base[i] == '\0', base's end found whitout a match for c
    je .not_found               ; return -1
    cmp BYTE [rsi + rax], dil   ; if base[i] == c, c found in base
    je .found                   ; return i
    inc rax                     ; i++
    jmp .loop

.not_found:
    mov rax, -1
    ret

.found:
    ret                         ; return rax = i


;;;;; check_duplicate fonction return 1 (True if duplicate found)
; int ft_check_duplicate(char *base)
; base      -> rdi
; return    -> rax
ft_check_duplicate:
    ; xor rax, rax                
    xor rcx, rcx                ; i = 0

.outer_loop:
    cmp BYTE [rdi + rcx], 0     ; if base[i] = '\0', all string have been checked without founding duplicate
    je return_false             ; -> return false (0)
    lea rdx, [rcx + 1]          ; j = i + 1 (lea = load effective address)

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
    mov rax, 1                  ; return 1
    ret

return_false:
    xor rax, rax                ; return 0
    ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; FT_ATOI_BASE ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; int ft_atoi_base(char *str, char *base)
; str		-> rdi
; base		-> rsi
; return	-> rax
ft_atoi_base:
    push rbx            ; save rbx, r12, r13, r14, r15 on the stack
    push r12
    push r13
    push r14
    push r15
    mov r12, rdi        ; r12 <- str
    mov rbx, rsi        ; rbx <- base
    xor r14, r14        ; i = 0

;;;;;;;;;;;;;; Checking if base is valid ;;;;;;;;;;;;;
    mov rdi, rbx                ; move base in rdi (first arg)
    call ft_strlen              ; get the lenght of base
    cmp rax, 2                  ; if ft_strlen(base) < 2, jump to error_exit
    jl .error_exit
    mov r13, rax                ; r13 <- ft_strlen(base)
    jmp .check_loop             ; else, continue to check_loop

.check_loop:
    movzx edi, BYTE [rbx + r14] ; edi <- base[i] (edi because isspace check int c and not char c)
    cmp dil, 0                  ; if base[i] == '\0', all base have been checked successufully for sign and space         
    je .check_duplicate         ; -> jump to next step : checking base for duplicate
    call ft_issign              ; check if '+' or '-' are present
    cmp rax, 1                  ; cmp the return value 
    je .error_exit              ; if equal (true), go to error_exit
    movzx edi, BYTE [rbx + r14] ; edi <- base[i] (reinit because rdi content could have change because of call)
    call ft_isspace             ; check if whitespace are present 
    cmp rax, 1                  ; cmp the return value
    je .error_exit              ; if equal (true), go to error_exit
    inc r14                     ; i++ 
    jmp .check_loop             ; return at the begining of the loop to check the next char

.check_duplicate:
    mov rdi, rbx                ; rdi <- base
    call ft_check_duplicate     ; check if they are duplicate in the base
    cmp rax, 1                  ; cmp the return value
    je .error_exit              ; if equal (true), go to error_exit
    jmp .skip_space             ; else, jump to next step : begin the actual convertion by skipping space

;;;;;;;;;;;;;; Skip whitespace at the begining of str ;;;;;;;;;;;;;
.skip_space:
    movzx edi, BYTE [r12]       ; edi <- *str
    call ft_isspace
    cmp rax, 0                  ; if isspace(*str) == False, go to next step : sign initialisation
    je .sign_init        
    inc r12                     ; else, str++
    jmp .skip_space             ; continue to check next char

;;;;;;;;;;;;;; Set sign ;;;;;;;;;;;;;
.sign_init:
    mov r14, 1                  ; r14 <- sign (initialise with 1)
    movzx edi, BYTE [r12]       ; edi <- *str
    call ft_issign              
    cmp rax, 0                  ; if ft_issign(*str) == False, go to next step : actual conversion
    je .convert_init
    cmp BYTE [r12], 45          
    je .set_negative            ; if *str == '-', go to set_negative to change sign value
    inc r12                     ; else, str++
    jmp .convert_init           ; go to next step : actual conversion

.set_negative:
    neg r14                     ; sign = -1
    inc r12                     ; str++
    jmp .convert_init           ; go to next step : actual conversion

;;;;;;;;;;;;;; Conversion ;;;;;;;;;;;;;
.convert_init:
    xor r15, r15                ; nbr = 0

.convert_loop:
    movzx edi, BYTE [r12]       ; edi <- *str
    mov rsi, rbx                ; rsi <- base
    call ft_inbase
    cmp rax, 0
    jl .done                    ; if ft_inbase(*str) < 0, *str not in base : conversion done
    imul r15, r13               ; nbr *= len(base) (imul = signed integer multiplication)
    add r15, rax                ; nbr = nbr + index in base
    inc r12                     ; str++
    jmp .convert_loop           ; continue to next character



.error_exit:
    xor rax, rax                ; set return value to 0
    jmp .end

.done:
    imul r15, r14               ; nbr *= sign
    mov rax, r15                ; rax <- nbr
    jmp .end
    
.end:
    pop r15
    pop r14
    pop r13
    pop r12                     
    pop rbx
    ret

