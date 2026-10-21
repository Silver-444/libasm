global ft_list_push_front
extern malloc


; void ft_list_push_front(t_list **begin_list, void *data)
; **begin_list	-> rdi
; *data     	-> rsi
ft_list_push_front:
    push rbx
    push r12
    sub rsp, 8              ; need to realign the stack, TO-DO : better understand        
    mov rbx, rdi            ; rbx <- **begin_list
    mov r12, rsi            ; r12 <- *data
    
    mov rdi, 16
    call malloc wrt ..plt   ; malloc to allocate memory for the node
    cmp rax, 0
    je .end
    mov [rax], r12          ; new_node->data = data
    mov rcx, [rbx]
    mov [rax + 8], rcx      ; new_node->next = *begin_list
    mov [rbx], rax          ; *begin_list = new_node
    
.end:
    add rsp, 8              ; restore the stack
    pop r12
    pop rbx
    ret
