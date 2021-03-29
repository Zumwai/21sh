NAME = 21sh
COM = clang
SRCS =	main.c \
		builtin/cd.c \
		builtin/env_list.c \
		builtin/environ.c \
		builtin/pwd_id_exit.c \
		builtin/type.c \
		builtin/cd_utils.c \
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
		tree/priority.c \
		utils/free.c \
		utils/freemore.c \
		utils/init.c \
		utils/init_env.c \
		utils/init_cmd.c \
		utils/handle_err.c \
		utils/misc.c \
		input/display.c \
		input/reader.c \
		input/input_utils.c \
		input/cursor.c \
		history/history.c \
		history/cpy_history.c \
		history/scroll.c \
		history/new_struct.c \
		controls/state.c \
		controls/next_io.c \
		controls/heredoc.c \
		controls/heredoc_utils.c \
		controls/controls.c \
		controls/arrow_movement.c \
		controls/edition.c \
		controls/ctrl_utils.c \
		controls/jump_words.c \
		controls/cut_paste.c \
		controls/parser.c \
		controls/parser_states.c \
		controls/parser_heredoc.c \
		controls/parser_utils.c \
		autocomplete/trie_construct.c \
		autocomplete/auto_trie.c \
		autocomplete/auto_utils.c \
		autocomplete/print.c \
		autocomplete/search_trie.c \
		autocomplete/check_existence.c \
		autocomplete/auto_state.c \
		autocomplete/check_ifdir.c \
		autocomplete/print_words.c \
		autocomplete/insert_word_trie.c \
		autocomplete/auto_dop_utils.c \
		autocomplete/filler.c \
		signals/sig_handler.c
DIR_SRCS = srcs/
SOURCE = $(addprefix $(DIR_SRCS), $(SRCS))
OBJECT = $(patsubst %.c,%.o,$(SOURCE))
DEPEND = $(patsubst %.c,%.d,$(SOURCE))
DIR_LIB = ./libft
LIB = $(DIR_LIB)/libft.a
HEAD = ./includes/
FLAGS = -g 
REMOVE = rm -rf
COMP_LIB = make -C $(DIR_LIB)
.PHONY: all clean re fclean


all: $(NAME)


$(NAME): $(OBJECT)
	$(COM) $(FLAGS) $(OBJECT) -o $(NAME) $(LIB) -ltermcap
%.o: %.c | $(LIB)
	$(COM) $(FLAGS) -c $< -o $@ -I$(DIR_LIB) -I$(HEAD) -MD
$(LIB): lib

lib:
	@$(COMP_LIB)
clean:
	$(REMOVE) $(OBJECT) && \
	$(REMOVE) $(DEPEND) && \
	$(MAKE) -sC $(DIR_LIB) clean

fclean : clean
	$(REMOVE) $(NAME) && \
	$(MAKE) -sC $(DIR_LIB) fclean

re: fclean all

include $(wildcard $(DEPEND))


