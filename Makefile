NAME = 21sh
COM = clang
HEAD_21sh = ./includes/
HEAD_L = ./libft/
HEAD_LIB = libft.h
HEAD_H = shell.h
HEAD_F = $(HEAD_21sh) \
		$(HEAD_L)
HEADS = $(addprefix -I, $(HEAD_F))
LIB_F = ./libft/
LIB = libft.a
LIB_D = $(addprefix $(LIB_F), $(LIB))
LIB_C = -L $(LIB_F) -lft
SOUS = ./src/
SOUS_FIL = main.c \
		   reader/handle_line.c \
		   cmd/command.c \
		   cmd/shelp_exec.c \
		   utils/handle_err.c \
		   utils/misc.c \
		   builtin/environ.c \
		   builtin/shelp_cd.c \
		   builtin/shelp_echo.c \
		   builtin/shelp_env.c \
		   builtin/shelp_ppid.c \
		   builtin/shelp_exit.c \
		   builtin/shelp_un_set_env.c \
		   builtin/shelp_help.c \
		   signals/shelp_signal.c \
		   input/controls.c \
		   input/display.c
OBJS = $(SOUS_FIL:.c=.o)
SOUS_F = $(addprefix $(SOUS), $(SOUS_FIL))
OBJS_F = $(addprefix $(BIN), $(OBJS))
BIN = ./bin/
TRASH = shell.h.gch \
		shell.dSYM
FLAGS = -g -fsanitize=address -fsanitize=undefined
REMOVE = rm -rf

.PHONY: all clean re

all: $(NAME)
$(NAME): $(OBJS_F) $(HEAD_21sh)$(HEAD_H) $(LIB_D)
	$(COM) $(FLAGS) -o $@ $(OBJS_F) $(HEADS) $(LIB_C) -ltermcap
$(LIB_D): $(LIB_F)
	$(MAKE) -sC $(LIB_F) all
$(BIN)%.o: $(SOUS)%.c
	mkdir -p $(BIN) && \
	mkdir -p $(BIN)/builtin && \
	mkdir -p $(BIN)/cmd && \
	mkdir -p $(BIN)/reader && \
	mkdir -p $(BIN)/utils && \
	mkdir -p $(BIN)/signals && \
	mkdir -p $(BIN)/input && \
	$(COM) $(FLAGS) -c $< -o $@ $(HEADS)
clean:
	@$(REMOVE) $(OBJS_F) $(BIN) $(TRASH)
	@make -sC $(LIB_F) clean
fclean: clean
	@$(REMOVE) $(NAME)
	@make -sC $(LIB_F) fclean
re: fclean all
