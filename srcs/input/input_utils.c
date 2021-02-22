#include "sh.h"



int key_exit(struct termios old_tty, t_term *pos, __attribute((unused))t_yank *buffer)
{
	tcsetattr(STDIN_FILENO, TCSADRAIN, &old_tty);
//	if (pos->new)
//		set_free_null(pos->new);
	free_input_line(&pos);
//	free(pos->new);
//	pos->new = NULL;
//	set_free_all(NULL, buffer);
	return(-2);
}

static void get_coordinates(int *start_y, int *start_x)
{
	char	buf[32];
	size_t	index = 0;
	int		red = 0;
	ft_bzero(buf, 32);
	if (write(1, "\033[6n", 4) != 4)
		handle_exit_errors("Write returns Error");
	while (index < sizeof(buf) - 1)
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
	bzero(pos, sizeof(t_term));
	pos->main = NULL;
	get_coordinates(&pos->y, &pos->x);
	if (!(pos->store = (t_scroll *)malloc(sizeof(t_scroll))))
		handle_exit_errors("Malloc returned NULL");
	bzero(pos->store, sizeof(t_scroll));
	if (!pos->main)
	{
		if (!(pos->main = (t_actual *)malloc(sizeof(t_actual))))
			handle_exit_errors("Malloc returned NULL");
		bzero(pos->main, sizeof(t_actual));
	}
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

t_actual	*create_main_line(void)
{
	t_actual	*new;

	if (!(new = (t_actual *)malloc(sizeof(t_actual))))
		handle_exit_errors("Malloc returned NULL");
	new->hdoc = NULL;
	new->line = NULL;
	new->state = 0;
	return new;
}

int		putchar_like(int n)
{
	size_t	size;

	size = write(STDIN_FILENO, &n, 1);
	return (size);
}

char	*concat_symbol(char *frst, char *scnd, char c)
{
	char	*new;

	new = ft_strnew(ft_strlen(frst) + ft_strlen(scnd) + 2);
	ft_strcpy(new, frst);
	ft_strcat(new, &c);
	ft_strcat(new, scnd);
	return new;
}