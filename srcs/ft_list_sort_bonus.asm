; void ft_list_sort(t_list **begin_list, int (*cmp)())
; **begin_list	-> rdi
; (*cmp)()      -> rsi

;(*cmp)(list_ptr->data, list_other_ptr->data)
; list_ptr	        -> rdi
; list_other_ptr    -> rsi

global ft_list_sort

ft_list_sort:
    push rbx
    push r12
    push r13
    push r14
    sub rsp, 8
    cmp rdi, 0          ; if begin_list == NULL, exit
    je .end
    cmp QWORD [rdi], 0  ; if *begin_list == NULL, exit 
    je .end 
    mov r12, rsi        ; r12 <- cmp()
    mov r13, rdi        ; r13 <- begin_list

.outer_loop:
    mov r14, 0          ; swapped = 0
    mov rbx, [r13]      ; rbx <- begin_list

.inner_loop:
    cmp QWORD [rbx + 8], 0    ; if node->next == NULL, go to .check_swapped
    je .check_swapped
    mov rdi, [rbx]      ; rdi <- node->data
    mov rsi, [rbx + 8]  ; rsi <- node->next
    mov rsi, [rsi]      ; rsi <- node->next->data
    call r12            ; call cmp(node->data, node->next->data)
    cmp eax, 0          ; if return_value <= 0, don't swap
    jle .no_swap
    mov rcx, [rbx]
    mov rdx, [rbx + 8]
    mov r8, [rdx]
    mov [rbx], r8       ; node->data = node->next->data
    mov [rbx + 8], rcx  ; node->next->data = node->data
    mov r14, 1          ; swapped = 1

.no_swap:
    mov rbx, [rbx + 8]  ; node = node->next
    jmp .inner_loop

.check_swapped:
    cmp r14, 0          ; if swapped != 0, go to outer_loop
    jne .outer_loop

.end:
    add rsp, 8
    pop r14
    pop r13
    pop r12
    pop rbx
    ret

; void ft_list_sort(t_list **begin_list, int (*cmp)())
; {
;     if (!begin_list || !*begin_list)
;         return;
;     int swapped = 1;
;     while (swapped)
;     {
;         swapped = 0;
;         t_list *cur = *begin_list;
;         while (cur->next)
;         {
;             if (cmp(cur->data, cur->next->data) > 0)
;             {
;                 void *tmp = cur->data;
;                 cur->data = cur->next->data;
;                 cur->next->data = tmp;
;                 swapped = 1;
;             }
;             cur = cur->next;
;         }
;     }
; }
