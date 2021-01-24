NAME = 21sh
COM = clang
SRCS =	main.c \
		builtin/cd.c \
		builtin/env_list.c \
		builtin/environ.c \
		cmd/get_cmd.c \
		exec/builtin_fd.c \
		exec/execute.c \
		tokens/ampersand.c \
		tokens/eject_redirect.c \
		tokens/flag.c \
		tokens/parse_tokens.c \
		tokens/pipe.c \
		tokens/semantica.c \
		tokens/token.c \
		tree/tree_ft.c \
		tree/tree.c \
		utils/free.c \
		utils/init.c \
		utils/handle_err.c \
		utils/misc.c \
		input/controls.c \
		input/display.c \
		input/reader.c \
		input/arrow_movement.c \
		input/edition.c \
		input/jump_words.c \
		input/input_utils.c \
		input/cut_paste.c \
		input/history.c
DIR_SRCS = srcs/
SOURCE = $(addprefix $(DIR_SRCS), $(SRCS))
OBJECT = $(patsubst %.c,%.o,$(SOURCE))
DIR_LIB = libft
LIB = $(DIR_LIB)/libft.a
INCLUDES = ./includes/
FLAGS = -g 
REMOVE = -rm -rf


.PHONY: all clean re

all: $(NAME)

$(NAME): $(OBJECT) $(LIB) 
	$(COM) $(FLAGS) $(OBJECT) -o $(NAME) -L ./libft -lft -ltermcap
%.o: %.c
	$(COM) $(FLAGS) -c $< -o $@ -I$(DIR_LIB) -I$(INCLUDES) 
$(LIB):
	$(MAKE) -sC $(DIR_LIB) all
clean:
	$(REMOVE) $(OBJECT)
fclean : clean
	$(REMOVE) $(NAME)
re: all clean
