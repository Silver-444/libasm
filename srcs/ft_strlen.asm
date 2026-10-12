global  _ft_strlen

section .text

_ft_strlen: 
        mov rax, 0
        jmp loop

loop:   
        cmp BYTE [rdi + rax], 0 ; compare byte value with 0
        je  exit                ; jump to exit if cmp equal
        inc rax                 ; increment rax
        jmp loop                ; jump at the begining of loop

exit: 
        ret
