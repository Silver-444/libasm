; int ft_atoi_base(char *str, char *base)
; *str		-> rdi
; *base		-> rsi
; return	-> rax
global  ft_atoi_base
extern  ft_strlen

ft_atoi_base:
    ret