NAME = 21sh
COM = clang
SRCS =	main.c \
		builtin/cd.c \
		builtin/env_list.c \
		builtin/environ.c \
		builtin/pwd_id_exit.c \
		cmd/get_cmd.c \
		exec/builtin_fd.c \
		exec/execute.c \
		exec/build_path.c \
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
		input/display.c \
		input/reader.c \
		input/history.c \
		input/input_utils.c \
		controls/state.c \
		controls/controls.c \
		controls/arrow_movement.c \
		controls/edition.c \
		controls/jump_words.c \
		controls/cut_paste.c \
		autocomplete/trie_construct.c \
		autocomplete/auto_trie.c \
		autocomplete/auto_utils.c
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
%.o: %.c $(LIB)
	$(COM) $(FLAGS) -c $< -o $@ -I$(DIR_LIB) -I$(INCLUDES) 
$(LIB):
	$(MAKE) -sC $(DIR_LIB) all
clean:
	$(REMOVE) $(OBJECT) && \
	$(MAKE) -sC $(DIR_LIB) clean
fclean : clean
	$(REMOVE) $(NAME) && \
	$(MAKE) -sC $(DIR_LIB) fclean
re: fclean all
