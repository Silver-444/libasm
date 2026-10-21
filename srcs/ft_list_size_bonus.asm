; unsigned int ft_list_size(t_list *begin_list)
; *begin_list	-> rdi
; return	    -> eax (unsigned int)

global ft_list_size

ft_list_size:
    xor eax, eax    ; i = 0

.loop:
    cmp rdi, 0          ; if begin_list == NULL, exit
    je .exit
    mov rdi, [rdi + 8]  ; begin_list = begin_list->next
    inc eax             ; i++
    jmp .loop

.exit:
    ret
