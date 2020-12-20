#ifndef CONTROL_H
# define CONTROL_H

# include "shell.h"
# include <stdbool.h>
# include <sys/ioctl.h>
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

typedef struct s_yank
{
	char		*yanked;
	int			size;
}				t_yank;
typedef struct s_term
{
	int			x;
	int			y;
	int			delta_x;
	int			delta_y;
	int			index;
	int			prompt; //dont need yet
	int			buf_size;
	char		*new;
	t_yank		*yank;
	t_env		*ptr;
}				t_term;

/*
	CONTROLS
*/

int 	read_key(long long key, t_term *pos, struct termios old, t_env *ptr, t_yank *buf);
//void	init_tty(void);
char	*get_input(t_env *ptr, t_yank *buffer);
void	draw_cursor_line(t_term *pos);
void	move_left(t_term *pos);
void 	move_right(t_term *pos);
void 	change_line_down(t_term *pos);
void 	change_line_up(t_term *pos);
void	backspace_char(t_term *pos);
void	insert_char (t_term *pos, char c);
void	delete_char(t_term *pos);
void	go_prev_word(t_term *pos);
void 	go_next_word(t_term *pos);
void	key_exit(struct termios old_tty, t_term *pos, t_env *ptr, t_yank *buffer);
char	*get_buf_line(char **line, int *size, int increase);


#endif