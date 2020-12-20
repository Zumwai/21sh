#include "shell.h"

char	*get_buf_line(char **line, int *size, int increase)
{
	char	*new;
	int		diff;

	struct winsize dimensions;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	new = ft_strnew(*size + dimensions.ws_col + increase);
	if (new == NULL)
		handle_exit_errors("Malloc returned NULL"); // add limitation to the input later
	*size += dimensions.ws_col + increase;
	if (*line != NULL)
	{
		ft_strcpy(new, *line);
		free(*line);
		*line = NULL;
	}
	*line = new;
	return (*line);
}

static void coordinates(int *start_y, int *start_x)
{
	char	buf[8];
	int		red = 0;
	ft_bzero(buf, 8);
	ft_putstr_size("\033[6n", 4);
	red = read(STDIN_FILENO, buf, 4);
	buf[red] ='\0';
	int	i = 0;
	while (buf[i] && !ft_isdigit(buf[i]))
		i++;
	*start_y = ft_atoi(&buf[i]);
	while(buf[i] && ft_isdigit(buf[i]))
		i++;
	*start_x = ft_atoi(&buf[i]);
			ft_bzero(buf, 8);
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
	ft_putstr_size("shelp$>", 7);
	pos.index = 0;
	pos.prompt = ft_strlen("shelp$>");
	coordinates(&pos.y, &pos.x);
	pos.x += pos.prompt;
	pos.delta_x = 0;
	pos.delta_y = 0;
	pos.buf_size = 0;
	pos.new = NULL;
	return (pos);
}

char	*get_input(t_env *ptr, t_yank *buffer)
{
	size_t		red;
	struct termios	old_tty;
	long		key;
	t_term	pos;

	ft_bzero(&old_tty, sizeof(struct termios));
	pos = init_prompt(&old_tty);
	key = 0;
	red = 0;
	while (1)
	{
			if (!pos.new)
				pos.new = get_buf_line(&pos.new, &pos.buf_size, 20);
			if (pos.index + 2 >= pos.buf_size)
				pos.new = get_buf_line(&pos.new, &pos.buf_size, 20);
			red = read(STDIN_FILENO, &key, sizeof(key));
	//		printf("%ld\n", key);
			if (read_key(key, &pos, old_tty, ptr, buffer) == -1)
			{
				ft_putchar_fd('\n', 1); // shoud've move cursor before \n
				break ;
			}
			draw_cursor_line(&pos);
			red = 0;
			key = 0;
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &old_tty);
	return (pos.new);
}