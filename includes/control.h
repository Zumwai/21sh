#ifndef CONTROL_H
# define CONTROL_H


# include <stdbool.h>

# include "definition.h"
# include <termios.h>
# include <termcap.h>


/*
	CONTROLS
*/

int 		read_key(long long key, t_term *pos, t_yank *buf, t_env **env);
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
int 		key_exit(t_term *pos, __attribute((unused))t_yank *buffer);
char		*get_buf_line(char **line, int *size, int increase);
void		cut_word(t_term *pos, t_yank *buffer);
void		cut_after(t_term *pos, t_yank *buffer);
void		cut_before(t_term *pos, t_yank *buffer);
void		yank_buffer(t_term *pos, char *line);
t_term		*create_new_io_struct(t_actual **line);
t_history	*save_history(t_yank *buffer);
void 		free_input_line(t_term **input);
void 		free_history(t_history **history);
void		envoke_history(t_yank *buffer, int key);
char		*ft_strdup_size(char *old, size_t size);
void		copy_word(t_term *pos, t_yank *buffer);
char		*handle_input_stream(t_yank *buffer, t_env **env, int *fail);
int			putchar_like(int n);
void		handle_empty_error(char *name, char *description);
int			handle_return_error(int num, char *name);
void		handle_exit_errors(char *description);
void		ft_putstr_size(char *line, ssize_t size);
char		*concat_lines(t_term *input);
void	free_storage(t_term *curr);
int		determine_state(char *line, int prev, t_term *pos);
char	*determine_glue(char *line, t_term *pos, int index);
t_actual	*create_main_line(void);
int		consult_state(t_term *curs);
void	save_coord_hdoc(t_norm *norm);
int		update_hdoc_list(t_hdoc **list, char *line);
t_hdoc	*create_new_hdoc(void);
t_hdoc	*clone_hdoc(t_hdoc *old);
void       update_coord(t_term *pos);
extern int			set_quotes(char c, int quotes);
extern t_term *get_last_pos(t_term *pos);
extern int is_printable(char key);
int		determine_next_io_step(t_term *curs, int ret, t_norm *norm);
void	move_cursor(t_term *pos, long long key);
void	set_cursor(t_term *pos);
void	correct_y(t_term *pos);
void	handle_child_signal(int sig);
void	handle_main_signal(int sig);
void	handle_all_signals(int pid);
#endif