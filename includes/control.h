#ifndef CONTROL_H
# define CONTROL_H

//# include "shell.h"

# define LEFT	4479771
# define RIGHT	4414235
# define ENTER	10
# define BACKSPACE 127
# define DELETE	2117294875
# define START	4741915
# define END	4610843

typedef struct s_term
{
	int			x;
	int			y;
	int			index;
	int			prompt;
}				t_term;


#endif