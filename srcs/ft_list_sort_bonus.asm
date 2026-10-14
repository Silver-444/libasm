; void ft_list_sort(t_list **begin_list, int (*cmp)())
; **begin_list	-> rdi
; (*cmp)()      -> rsi

;(*cmp)(list_ptr->data, list_other_ptr->data)
; list_ptr	        -> rdi
; list_other_ptr    -> rsi