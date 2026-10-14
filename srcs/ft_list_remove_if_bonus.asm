; void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *))
; **begin_list	        -> rdi
; *data_ref             -> rsi
; (*cmp)()              -> rdx
; (*free_fct)(void *)   -> rcx

;(*cmp)(list_ptr->data, data_ref)
; list_ptr  -> rdi
; data_ref  -> rsi
; return    -> rax

;(*free_fct)(list_ptr->data)
; list_ptr  -> rdi
; return    -> rax
