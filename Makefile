NAME = libasm

# ----------- Directories -----------
OBJS_DIR = objs/
SRCS_DIR = srcs/

vpath %.asm $(SRCS_DIR)

# ----------- Files -----------
ASM_FILES =	ft_strlen.asm \
			ft_strcpy.asm \
# 			ft_strcmp.asm \
# 			ft_write.asm \
# 			ft_read.asm \
# 			ft_strdup.asm

# ASM_FILE_BONUS = 

OBJS = $(addprefix $(OBJS_DIR), $(ASM_FILES:.asm=.o))

# ----------- Compilation -----------
ASM = nasm
ASM_FLAGS = -f elf64

CC = gcc
CC_FLAGS = -Wall -Wextra -Werror


# ============ Rules ============
all: $(NAME)

$(NAME) : $(OBJS)
	ar rcs $(NAME) $(OBJS)

$(OBJS_DIR)%.o : %.asm
	mkdir -p $(OBJS_DIR)
	$(ASM) $(ASM_FLAGS) $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re