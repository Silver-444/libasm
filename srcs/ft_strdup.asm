global  ft_strdup
extern  ft_strlen
extern  ft_strcpy
extern  malloc

section .text

ft_strdup:
    push rdi                ; push *s1 on the program stack to save while calling other function
    call ft_strlen          ; ft_strlen(*s1) => rax = len(*s1)
    inc rax                 ; add 1 to rax for '\0'
    mov rdi, rax            ; move len(*s1) + 1 in rdi(first arg) 
    call malloc wrt ..plt   ; malloc(len(*s1)) -> rax = *dst
    cmp rax, 0              ; check malloc error
    je .malloc_error  
    mov rdi, rax            ; put *dst(malloc return) in first arg
    pop rsi                 ; pop *s1 save on the stack to rsi(second arg)
    call ft_strcpy          ; ft_strcpy(*dst, *s1) -> *dst
    ret

.malloc_error:
    pop rdi                 ; pop the value saved in the stack in all cases
    ret


; char	*ft_strdup(const char *s1);
; *s1		-> rdi
; return	-> rax
