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
	pos->y = 0;
	pos->x = 0;
	get_coordinates(&pos->y, &pos->x);
	pos->index = 0;
	pos->prompt = ft_strlen("shelp$>");
	pos->x += pos->prompt;
	pos->state = 0;
	pos->delta_x = 0;
	pos->delta_y = 0;
	pos->buf_size = 0;
	pos->heredoc = 0;
	pos->store = NULL;
	pos->glue = false;
	pos->new = NULL;
	pos->next = NULL;
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
/*
char	*glue_lines(t_term *input)
{
	t_term *curs;
	char	*line;

	curs = input;
	line = NULL;
	while (curs)
	{
		tmp = line;

	}
}
*/
char	*concat_lines(t_term *input)
{
	t_term	*curs;
	char	*line;
	char	*tmp;

	curs = input;
	line = NULL;
	while(curs)
	{
		tmp = line;
		if (curs->new) {
			
			//printf("%s\n", curs->new);
			if (!line && curs->new)
				line = ft_strdup(curs->new);
			else
				line = ft_strjoin(line, curs->new);
			free(tmp);
			printf("%s\n", line);
		}
		curs = curs->next;
	}
	if (line && ft_strlen(line) > 4095)
	{
		handle_empty_error("usage: length", "input line too long, must be under 4096\n");
		set_free_null(&line);
		return (NULL);
	}
	return (line);
}
