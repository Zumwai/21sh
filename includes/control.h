#ifndef CONTROL_H
# define CONTROL_H

# include "shell.h"
# include <stdbool.h>
# include <sys/ioctl.h>

# define LEFT	4479771
# define RIGHT	4414235
# define ENTER	10
# define BACKSPACE 127
# define DELETE	2117294875
# define START	4741915
# define END	4610843
# define L_WORD 74995417045787
# define R_WORD 73895905418011

typedef struct s_term
{
	int			x;
	int			y;
	int			cur_x;
	int			cur_y;
	int			index;
	int			prompt;
	int			buf_size;
	bool		current;
	bool		down;
}				t_term;

/*
	CONTROLS
*/

int 	read_key(char *new, long long key, t_term *pos, struct termios old);
void	init_tty(void);
char	*get_input(void);

#endif