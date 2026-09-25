ASM_FILE =	ft_strlen.asm \
			ft_strcpy.asm \
			ft_strcmp.asm \
			ft_write.asm \
			ft_read.asm \
			ft_strdup.asm

# ASM_FILE_BONUS = 

ASM = nasm
ASM_FLAGS = -f elf64

CC = gcc
CC_FLAGS = -Wall -Wextra -Werror


all:

clean:

fclean:

re:

.PHONY: all clean fclean re