#include "shell.h"

void ft_putstr_size(char *line, size_t size)
{
	write(1, line, size);
}

int key_exit(struct termios old_tty, t_term *pos, t_yank *buffer)
{
	tcsetattr(STDIN_FILENO, TCSADRAIN, &old_tty);
	if (pos->new)
		set_free_null(pos->new);
	set_free_all(NULL, buffer);
	return(1);
}