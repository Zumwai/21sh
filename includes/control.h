#ifndef CONTROL_H
# define CONTROL_H

# include "shell.h"
# include <stdbool.h>
# include <sys/ioctl.h>
//# include <wchar.h>
//# include <locale.h>


# define LEFT	4479771
# define RIGHT	4414235
# define ENTER	10
# define BACKSPACE 127
# define DELETE	2117294875
# define START	4741915
# define END	4610843
# define L_WORD 74995417045787
# define R_WORD 73895905418011
# define L_UP	71696882162459
# define L_DOWN	72796393790235

typedef struct s_term
{
	int			x;
	int			y;
	int			delta_x;
	int			delta_y;
	int			index;
	int			prompt;
	int			buf_size;
	char		*yanked;
}				t_term;

/*
	CONTROLS
*/

int 	read_key(char *new, long long key, t_term *pos, struct termios old);
void	init_tty(void);
char	*get_input(void);
void	draw_cursor_line(char *new, t_term *pos);
void	move_left(t_term *pos);
void 	move_right(t_term *pos);
void 	change_line_down(t_term *pos);
void 	change_line_up(t_term *pos);
void	backspace_char(char *new, t_term *pos);
void	insert_char (char *new, t_term *pos, char c);
void	delete_char(char *new, t_term *pos);
void	go_prev_word(char *new, t_term *pos);
void 	go_next_word(char *new, t_term *pos);
void	 key_exit(struct termios old_tty);


#endif