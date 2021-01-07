#include "shell.h"

void ft_putstr_size(char *line, size_t size)
{
	write(1, line, size);
}

int key_exit(struct termios old_tty, t_term *pos, __attribute((unused))t_yank *buffer)
{
	tcsetattr(STDIN_FILENO, TCSADRAIN, &old_tty);
//	if (pos->new)
//		set_free_null(pos->new);
	free(pos->new);
	pos->new = NULL;
//	set_free_all(NULL, buffer);
	return(-2);
}

static void get_coordinates(int *start_y, int *start_x)
{
	char	buf[24];
	size_t	index = 0;
	int		red = 0;
	ft_bzero(buf, 24);
	if (write(1, "\033[6n", 4) != 4)
		handle_exit_errors("Write returns Error");
	while (index < sizeof(buf) - 1) //rewrite cycle, fixed y, but not x
	{
		if (read(STDIN_FILENO, buf + index, 1) != 1)
			break ;
		if (buf[index] == 'R')
			break ;
		index++;
	}
	buf[red] ='\0';
	if (!ft_isdigit(*(buf + 2)))
		return ;
	*start_y = ft_atoi(buf + 2);
	*start_x = 0;
}

t_term	*create_new_io_struct(void)
{
	t_term			*pos;
	if (!(pos = (t_term *)malloc(sizeof(t_term))))
		handle_exit_errors("Malloc returned NULL");
	get_coordinates(&pos->y, &pos->x);
	pos->index = 0;
//	pos->y = 0;
	pos->prompt = ft_strlen("shelp$>");
	pos->x += pos->prompt;
	pos->state = 0;
	pos->delta_x = 0;
	pos->delta_y = 0;
	pos->buf_size = 0;
	pos->new = NULL;
	pos->next = NULL;
	pos->heredoc = 0;
	pos->substr = NULL;
	pos->prev = NULL;
	return (pos);
}

char	*ft_strdup_size(char *old, size_t size)
{
	char	*new;

	if (!(new = ft_strnew(size)))
		handle_exit_errors("Malloc returned NULL");
	ft_strcpy(new,old);
	return (new);
}
