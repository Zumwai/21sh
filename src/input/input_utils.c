#include "shell.h"

void ft_putstr_size(char *line, size_t size)
{
	write(1, line, size);
}

void key_exit(struct termios old_tty, t_term *pos, t_env *ptr, t_yank *buffer)
{
	tcsetattr(STDIN_FILENO, TCSADRAIN, &old_tty);
	if (pos->new)
		set_free_null(pos->new);
	set_free_all(ptr, buffer);
	exit(1);
}