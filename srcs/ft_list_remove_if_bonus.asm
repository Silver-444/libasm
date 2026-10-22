; void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *))
; **begin_list	        -> rdi
; *data_ref             -> rsi
; (*cmp)()              -> rdx
; (*free_fct)(void *)   -> rcx

;(*cmp)(list_ptr->data, data_ref)
; list_ptr->data    -> rdi
; data_ref          -> rsi
; return            -> rax

;(*free_fct)(list_ptr->data)
; list_ptr  -> rdi
; return    -> rax

BITS 64
default rel


global  ft_list_remove_if
extern  free

ft_list_remove_if:
    push rbx
    push r12
    push r13
    push r14
    push r15
    cmp rdi, 0
    je .end
    mov rbx, rdi        ; rbx <- begin-list
    mov r12, rsi        ; r12 <- data_ref
    mov r13, rdx        ; r13 <- cmp()
    mov r14, rcx        ; r14 <- free_fct()
    mov r15, rdi        ; node = begin_list

.loop:
    mov r15, [rbx]
    cmp r15, 0  ; if node == NULL, go to .end
    je .end
    mov rdi, [r15]  ; rdi <- node->data
    mov rsi, r12    ; rsi <- node->data
    call r13
    cmp rax, 0
    je .next_node

.remove:
    mov rax, [r15 + 8]  ; rax <- node->next
    mov [rbx], rax      ; update begin
    mov rdi, [r15]      ; rdi <- node->data
    call r14            ; free_ft(node->data)
    mov rdi, r15
    call free wrt ..plt ; free(node)
    jmp .loop


.next_node:
    lea rbx, [r15 + 8]  ; node = &node->next
    jmp .loop

.end:
    pop r15
    pop r14
    pop r13
    pop r12
    pop rbx
    ret
