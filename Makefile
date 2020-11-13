NAME = minishell
COM = gcc
HEAD_MINI = ./includes/
HEAD_L = ./libft/
HEAD_LIB = libft.h
HEAD_H = minishell.h
HEAD_F = $(HEAD_MINI) \
		$(HEAD_L)
HEADS = $(addprefix -I, $(HEAD_F))
LIB_F = ./libft/
LIB = libft.a
LIB_D = $(addprefix $(LIB_F), $(LIB))
LIB_C = -L $(LIB_F) -lft
SOUS = ./src/
SOUS_FIL = main.c \
		   handle_line.c \
		   command.c \
		   shelp_exec.c \
		   handle_err.c \
		   environ.c \
		   misc.c \
		   shelp_cd.c \
		   shelp_echo.c \
		   shelp_un_set_env.c \
		   shelp_signal.c \
		   shelp_env.c \
		   shelp_ppid.c \
		   shelp_exit.c \
		   shelp_help.c
OBJS = $(SOUS_FIL:.c=.o)
SOUS_F = $(addprefix $(SOUS), $(SOUS_FIL))
OBJS_F = $(addprefix $(BIN), $(OBJS))
BIN = ./bin/
TRASH = minishell.h.gch \
		minishell.dSYM
FLAGS = -Wall -Wextra -Werror
REMOVE = rm -rf

.PHONY: all clean re

all: $(NAME)
$(NAME): $(OBJS_F) $(HEAD_MINI)$(HEAD_H) $(LIB_D)
	$(COM) $(FLAGS) -o $@ $(OBJS_F) $(HEADS) $(LIB_C)
$(LIB_D): $(LIB_F)
	$(MAKE) -sC $(LIB_F) all
$(BIN)%.o: $(SOUS)%.c
	mkdir -p $(BIN) && \
	$(COM) $(FLAGS) -c $< -o $@ $(HEADS)
clean:
	@$(REMOVE) $(OBJS_F) $(BIN) $(TRASH)
	@make -sC $(LIB_F) clean
fclean: clean
	@$(REMOVE) $(NAME)
	@make -sC $(LIB_F) fclean
re: fclean all
