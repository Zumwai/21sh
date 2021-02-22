#ifndef CONTROL_H
# define CONTROL_H


# include <stdbool.h>

# include "definition.h"
# include <termios.h>
# include <termcap.h>


/*
	CONTROLS
*/

int 		read_key(long long key, t_term *pos, struct termios old, t_yank *buf, t_env **env);
//void		init_tty(void);
int			display_input(t_term *pos, int delta);
void		move_left(t_term *pos);
void 		move_right(t_term *pos);
void 		change_line_down(t_term *pos);
void 		change_line_up(t_term *pos);
void		backspace_char(t_term *pos);
void		insert_char (t_term *pos, char c);
void		delete_char(t_term *pos);
void		go_prev_word(t_term *pos);
void 		go_next_word(t_term *pos);
int key_exit(struct termios old_tty, t_term *pos, __attribute((unused))t_yank *buffer);
char		*get_buf_line(char **line, int *size, int increase);
void		cut_word(t_term *pos, t_yank *buffer);
void		cut_after(t_term *pos, t_yank *buffer);
void		cut_before(t_term *pos, t_yank *buffer);
void		yank_buffer(t_term *pos, char *line);
t_term		*create_new_io_struct(void);
t_history	*save_history(t_yank *buffer);
void 		free_input_line(t_term **input);
void free_history(t_history **history);
void	envoke_history(t_yank *buffer, int key);
char	*ft_strdup_size(char *old, size_t size);
void	copy_word(t_term *pos, t_yank *buffer);
char	*handle_input_stream(t_yank *buffer, t_env **env);
int		putchar_like(int n);
void	handle_empty_error(char *name, char *description);
int		handle_return_error(int num, char *name);
void	handle_exit_errors(char *description);
void ft_putstr_size(char *line, size_t size);
char	*concat_lines(t_term *input);
t_trie    *find_best_match(char *orig, t_env **env);
int	autocomplete(t_term *pos, t_env **env, t_yank *buf);
void free_trie_node(t_trie* node);
t_trie    *construct_trie(char **orig, t_env **env, int source);
void free_trie_node(t_trie* node);
int  convert_asc_value(char c);
extern t_trie *create_trie_node(char c);
extern t_trie **init_array(void);
extern char *builtin_list(int i);
int		determine_state(char *line, int prev, t_term *pos);
char	*determine_glue(char *line, t_term *pos, int index);
t_actual	*create_main_line(void);
int		consult_state(t_term *curs);
void	save_coord_hdoc(t_hdoc **lst, int i, int size);
int	update_hdoc_list(t_hdoc **lst, char *line);
int		verify_char_heredoc(char c);
t_hdoc *clone_hdoc(t_hdoc *old);
#endif