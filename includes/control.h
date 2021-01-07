#ifndef CONTROL_H
# define CONTROL_H

# include <stdbool.h>
# include <sys/ioctl.h>
# include "shell.h"
//# include <wchar.h>
//# include <locale.h>


# define LEFT	4479771			//left arrow
# define RIGHT	4414235			//right
# define ENTER	10				//return
# define BACKSPACE 127			//backspace
# define DELETE	2117294875		//delete
# define START	4741915			//home
# define END	4610843			//end
# define L_WORD 74995417045787	//ctrl + left arrow
# define R_WORD 73895905418011	//+ right
# define L_UP	71696882162459	//+ up
# define L_DOWN	72796393790235	//+ down
# define R_CUT	11				//ctrl + k
# define L_CUT	21				//ctrl + u
# define YANK	25				//ctrl + y
# define W_CUT	23				//ctrl + w
# define CLEAR	12				//ctrl + l
# define HISTORY_UP	4283163
# define HISTORY_DOWN	4348699



typedef struct 		s_term
{
	int				x;
	int				y;
	int				delta_x;
	int				delta_y;
	int				index;
	int				prompt; //dont need yet
	int				buf_size;
	int				state;
	int				heredoc;
	char			*substr;
	char			*new;
	struct s_term	*next;
	struct s_term	*prev;
}					t_term;

typedef struct 		s_history
{
	t_term				*line;	
	struct s_history	*next;
	struct s_history	*prev;
}					t_history;

typedef struct s_yank
{
	char			*yanked;
	int				size;
	int				counter;
	t_history		*hist_ptr;
	t_term			*input;
	t_term			*current;
	t_term			*saved;
	t_history		*history;
}					t_yank;

enum				e_state
{
	DEFAULT,
	QUOTES,
	DOUBLE_QUOTES,
	HEREDOC,
	POST_DOC
};

/*
	CONTROLS
*/

int 		read_key(long long key, t_term *pos, struct termios old, t_yank *buf);
//void		init_tty(void);
t_term		*get_input(t_yank *buffer);
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
int			key_exit(struct termios old_tty, t_term *pos, t_yank *buffer);
char		*get_buf_line(char **line, int *size, int increase);
void		cut_word(t_term *pos, t_yank *buffer);
void		cut_after(t_term *pos, t_yank *buffer);
void		cut_before(t_term *pos, t_yank *buffer);
void		yank_buffer(t_term *pos, t_yank *buffer);
t_term		*create_new_io_struct(void);
t_history	*save_history(t_yank *buffer);
<<<<<<< HEAD
void 		free_input_line(t_term *input);
void free_history(t_history **history);
t_term *copy_input_struct(t_term *current);
=======
void 		free_input_data(t_term **input);
void	 free_history(t_history **history);
void	envoke_history(t_yank *buffer, int key);
>>>>>>> 4268e47320fc63f172a6dfb4e0222cd70fc35079

#endif