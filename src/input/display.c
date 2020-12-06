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

static void draw_line(char *new, t_term __attribute__((unused))*pos)
{
	int		i  = 0;
	int		j = 8;
	struct winsize dimensions;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	ft_putstr_fd("shelp$>", 1);
	pos->y = 0;
	while (new[i])
	{
		ft_putchar_fd(new[i], 1);
		i++;
		if (i - j >= dimensions.ws_col)
		{
			j += i;
			pos->y++;
			pos->x = 0;
			tputs(tgetstr("do", NULL), 1, putchar_like);
			int k = 0;
			while (k++ <= dimensions.ws_col - 1)
					tputs(tgetstr("le", NULL), 1, putchar_like);
			//tputs(tgetstr("do", NULL), 1, putchar_like);
		}
	}
}

static void draw_cursor_line(char *new, t_term *pos)
{
	int i = 0;
	int	tmp = pos->index + pos->prompt;
	struct winsize dimensions;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	tputs(tgetstr("cb", NULL), 1, putchar_like);
	tputs(tgetstr("cd", NULL), 1, putchar_like);
	while (i++ < pos->prompt + pos->index - 1)
	{
		tputs(tgetstr("le", NULL), 1, putchar_like);
	}
	draw_line(new, pos);
//	int y = 0;
//	while (y++ < pos->y)
//			tputs(tgetstr("up", NULL), 0, putchar_like);
	while (tmp-- > pos->x)
			tputs(tgetstr("le", NULL), 0, putchar_like);
//	pos->y = 0;
}

static void coordinates(__attribute((unused))int *x, __attribute((unused))int *y)
{
	char	buf[7];
	int		red = 0;
	ft_bzero(buf, 7);
	red = read(STDIN_FILENO, buf, 7);
	*x = ft_atoi(&buf[2]);
	*y = ft_atoi(&buf[5]);
	printf("%d;%d\n", *x, *y);
//	printf("%d\n", red);
}
/*
static int getCursor(void) {
	int x = 0, y = 0;
	ft_putstr_fd("\033[6n", 1);
  	//scanf("\033[%d;%dR", &y, &x);
	coordinates(&x, &y);
	//printf("%d - x; %d - y", x, y);
   return (y);
}
*/
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
	tcsetattr(STDIN_FILENO, TCSADRAIN, &tty);	
	ft_putstr_fd("shelp$>", 1);
	pos.index = 0;
	pos.prompt = ft_strlen("shelp$>") + 1;
//	pos.y = getCursor();
	pos.y = 0;
	pos.x = pos.prompt;
	pos.down = 0;
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
	wchar_t abc = 0x20A1;
	setlocale(LC_CTYPE, "");
	printf("%lc\n", abc);
	while (1)
	{
		printf("%lc", abc);
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
