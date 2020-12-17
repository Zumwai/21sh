#include "shell.h"

void ft_putstr_size(char *line, size_t size)
{
	write(1, line, size);
}