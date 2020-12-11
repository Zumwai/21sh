#include "shell.h"

static char	*get_buf_line(char **line, int *size)
{
	char	*new;

	struct winsize dimensions;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	new = ft_strnew(*size + dimensions.ws_col + 10);
	*size += dimensions.ws_col + 10;
	if (*line != NULL)
	{
		ft_strcpy(new, *line);
		free(*line);
		*line = NULL;
	}
	return (new);
}

static void ft_putstr_size(char *new, int size)
{
	write(1, new, size);
}

static void coordinates(int *start_y, int *start_x)
{
	char	buf[8];
	int		red = 0;
	ft_bzero(buf, 8);
	ft_printf("\033[6n");
	red = read(STDIN_FILENO, buf, 8);
	buf[red] ='\0';
	int	i = 0;
	while (buf[i] && !ft_isdigit(buf[i]))
		i++;
	*start_y = ft_atoi(&buf[i]);
	while(buf[i] && ft_isdigit(buf[i]))
		i++;
	*start_x = ft_atoi(&buf[i]);
}

static void set_cursor(t_term *pos, t_term tmp)
{
	struct winsize dimensions;
	struct winsize tempo;
	int		ch_x = 0;
	int		ch_y = 0;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
//	coordinates(&pos->y, &pos->x);
	tempo = dimensions;
	if (pos->delta_x > pos->x)
	{
		pos->delta_x += pos->x;
		if (pos->delta_x < 0)
		{
			ch_y--;
			ch_x = dimensions.ws_col + pos->delta_x;
			pos->x = ch_x + 1;
		}
	}
	else
	{
		pos->x += pos->delta_x;
	}
	/*
	else if (pos->delta_x < pos->x)
	{
		pos->x +=pos->delta_x;
	}
	*/
	int j;
	if (pos->y == pos->start_y)
		j = 1;
	else
	{
		j = 0;
	}
	
	tputs (tgoto (tgetstr("cm", NULL), pos->x - j, pos->y + ch_y - 1), 1, putchar_like);
}

static int draw_line(char *new, t_term *pos, int remainder)
{
	int		printed = 0;
	int		curr = 0;
	struct winsize dimensions;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	curr = 0;
	if (pos->y== pos->start_y)
		curr = pos->prompt;
	if (dimensions.ws_col > remainder + curr)
	{
		ft_putstr_size(&new[pos->index - remainder], remainder);
		pos->x = remainder + curr;
		return (0);
	}
	else {
		printed = dimensions.ws_col - curr;
		ft_putstr_size(&new[pos->index - remainder], printed);
		tputs (tgoto (tgetstr("cm", NULL), 0, pos->y), 1, putchar_like);
		pos->y += 1;
		pos->x = 1;
		return (remainder - printed);
	}
}

static void draw_cursor_line(char *new, t_term *pos)
{
	int i = 0;
	int	rem = pos->index;
	int	fin = 0;
	int	tmp = pos->index + pos->prompt;
	struct winsize dimensions;
	t_term		temp = *pos;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	tputs (tgoto (tgetstr("cm", NULL), 0, pos->start_y - 1), 1, putchar_like);
	tputs(tgetstr("cb", NULL), 1, putchar_like);
	tputs(tgetstr("cd", NULL), 1, putchar_like);
	ft_putstr_fd("shelp$>", 1);
	while (1)
	{
		rem = (draw_line(new, pos, rem));
		if (rem == 0)
			break ;
	}
//	*pos = temp;
	set_cursor(pos, temp);
	*pos = temp;
//	tputs (tgoto (tgetstr("cm", NULL), pos->x - 1, pos->y - 1), 1, putchar_like);
}


static t_term init_prompt(struct termios *old_tty)
{
	
	struct termios	tty;
	t_term			pos;

	tcgetattr(STDIN_FILENO, old_tty);
	tcgetattr(STDIN_FILENO, &tty);
	tty.c_lflag &= ~(ECHO | ICANON);
//	tty.c_lflag &= ~(ECHO | IEXTEN | ISIG);
	tty.c_cc[VMIN] = 1;
	tty.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &tty);	
	ft_putstr_fd("shelp$>", 1);
	pos.index = 0;
	pos.prompt = ft_strlen("shelp$>") + 1;
	coordinates(&pos.start_y, &pos.start_x);
	pos.x += pos.prompt;
	pos.y = pos.start_y;
	pos.delta_x = 0;
	pos.delta_y = 0;
	pos.buf_size = 0;
//	tputs (tgoto (tgetstr("cm", NULL), 2, 2), 1, putchar_like);
	return (pos);
}

void		init_tty(void)
{
	int		res;
	char	*tty_name;

	if (!(tty_name = getenv("TERM")))
		handle_exit_errors("terminal type is not defined\n");
	if (!isatty(STDIN_FILENO))
		handle_exit_errors("should be run in a terminal\n");
	res = tgetent(NULL, tty_name);
	if (res < 0)
		handle_exit_errors("could not access the termcap data base\n");
	if (!res)
		handle_exit_errors("specify a valid terminal name with setenv\n");
}

char	*get_input(void)
{
	size_t		red;
	struct termios	old_tty;
	long long		key;
	char	*new;
	t_term	pos;

	ft_bzero(&old_tty, sizeof(struct termios));
	pos = init_prompt(&old_tty);
	key = 0;
	new = NULL;
	red = 0;
	while (1)
	{
			if (!new)
				new = get_buf_line(&new, &pos.buf_size);
			if (pos.index + 2 >= pos.buf_size)
				new = get_buf_line(&new, &pos.buf_size);
			red = read(STDIN_FILENO, &key, sizeof(key));
			if (read_key(new, key, &pos, old_tty) == -1)
			{
				ft_putchar_fd('\n', 1);
				break ;
			}
			draw_cursor_line(new, &pos);
			red = 0;
			key = 0;
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &old_tty);
	return (new);
}
