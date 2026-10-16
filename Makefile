NAME = libasm
TESTER_NAME = tester

# ----------- Directories -----------
OBJS_DIR = objs/
SRCS_DIR = srcs/
INC_DIR = include/

vpath %.asm $(SRCS_DIR)

# ----------- Files -----------
ASM_FILES =	ft_strlen.asm \
			ft_write.asm \
			ft_read.asm \
			ft_strcpy.asm \
			ft_strcmp.asm \
			ft_strdup.asm

# ASM_FILE_BONUS =	ft_atoi_base_bonus.asm \
					ft_list_push_front_bonus.asm \
					ft_list_size_bonus.asm \
					ft_list_sort_bonus.asm \
					ft_list_remove_if_bonus.asm

OBJS = $(addprefix $(OBJS_DIR), $(ASM_FILES:.asm=.o))
OBJS_BONUS = $(addprefix $(OBJS_DIR), $(ASM_FILES_BONUS:.asm=.o))

TESTER_FILE = test/tester.c

# ----------- Compilation -----------
ASM = nasm
ASM_FLAGS = -f elf64

CC = gcc
CC_FLAGS = -Wall -Wextra -Werror -g3


# ============ Rules ============
all: $(NAME)

$(NAME) : $(OBJS)
	ar rcs $(NAME) $(OBJS)

bonus: $(OBJS) $(OBJS_BONUS)
	ar rcs $(NAME) $(OBJS) $(OBJS_BONUS)

$(OBJS_DIR)%.o : %.asm
	mkdir -p $(OBJS_DIR)
	$(ASM) $(ASM_FLAGS) $< -o $@

test:
	$(CC) $(CC_FLAGS) -I $(INC_DIR) $(TESTER_FILE) -L. -l:$(NAME) -o $(TESTER_NAME)
	@./$(TESTER_NAME)

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all bonus test clean fclean re
