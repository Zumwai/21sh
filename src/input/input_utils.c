#include "shell.h"

void ft_putstr_size(char *line, size_t size)
{
	write(1, line, size);
}

void set_free_null(char *line)
{
	if (line)
		free(line);
	line = NULL;
}

void key_exit(struct termios old_tty)
{
	tcsetattr(STDIN_FILENO, TCSADRAIN, &old_tty);
	exit(1);
}