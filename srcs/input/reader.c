#include "sh.h"


char	*get_buf_line(char **line, int *size, int increase)
{
	char	*new;

	struct winsize dimensions = {0};

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	new = ft_strnew(*size + dimensions.ws_col + increase);
	if (new == NULL)
		handle_exit_errors("Malloc returned NULL"); // add limitation to the input later && invalid size
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


static t_term *init_prompt(struct termios *old_tty)
{
	struct termios	tty = {0};
	t_term			*pos = NULL;
	tcgetattr(STDIN_FILENO, old_tty);
	tcgetattr(STDIN_FILENO, &tty);
	//tty.c_lflag &= ~(ECHO | ICANON);
	tty.c_lflag &= ~(ECHO | ICANON | ISIG);
	//tty.c_oflag &= ~(TABDLY);
//	tty.c_lflag &= ~(ECHO | IEXTEN | ISIG);
	tty.c_cc[VMIN] = 1;
	tty.c_cc[VTIME] = 1;
	tcsetattr(STDIN_FILENO, TCSANOW, &tty);
	pos = create_new_io_struct();
	return (pos);
}

void	print_value_into_file(long long key, int x, int y)
{
	FILE *fptr = {0};
	fptr = fopen("input_log", "aw");
	if (key == '\n') {
			fprintf(fptr, "%s\n", "--------------------------------");
	} else {
		fprintf(fptr, "%lld: key, %d:x, %d:y", key, x, y);
	if (ft_ischar(key))
		fprintf(fptr, ";%c - char key", (char)key);
	fprintf(fptr, "%c", '\n');
	}
}


static long long	incapsulate_read(void)
{
	long long key;
	ssize_t	ret;
	ret = 0;
	key = 0;
	ret = read(STDIN_FILENO, &key, sizeof(key));
	if (ret == -1)
		handle_exit_errors("read ERRNO");
	return (key);
}

void	recalc_y(t_term *pos, int y)
{
	t_term	*curs;
	int		tmp;

	curs = pos;
	tmp = curs->y;
	curs->y = y;
	curs = curs->next;
	while (curs)
	{
		curs->y = y + curs->y - tmp;
		curs = curs->next;
	}
}

static t_term *get_input(t_yank *buffer, t_env **env)
{
	ssize_t		red;
	struct termios	old_tty;
	long long	key;
	int tmp;
	t_term	*pos;
//	t_term	*head;

	ft_bzero(&old_tty, sizeof(struct termios));
	pos = init_prompt(&old_tty);
	ft_putstr_size("shelp$>", 7);
//	head = pos;
	key = 0;
	red = 0;
	buffer->current = pos;
	buffer->saved = NULL;
	while (1)
	{
			key = incapsulate_read();
		//	print_value_into_file(key, buffer->current->x, buffer->current->y);
			red = (read_key(key, buffer->current, old_tty, buffer, env));
		//	printf("%lld\n", key);
			if (red == -1 || red == -5) 
				break ;
			if (red == -2)
				return NULL;
			if (red == HIST_UP || red == HIST_D) {
				int tmp = buffer->current->y;
				envoke_history(buffer, red);
				recalc_y(buffer->current, tmp);
			}
			red = 0;
			key = 0;
			display_input(buffer->current, 0);
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &old_tty);
	return (buffer->current);
}

char	*handle_input_stream(t_yank *buffer, t_env **env)
{
	char	*line;

	line =  NULL;
	buffer->current = get_input(buffer, env);
	if (buffer->current) {
		line = concat_lines(buffer->current);
		//printf("%s - line in handle input\n", line);
		buffer->history = save_history(buffer);
		free_input_line(&buffer->current);
		if (buffer->saved)
			free_input_line(&buffer->saved);
	}
	//printf("%s\n", line);
	return (line);
}