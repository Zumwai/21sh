#include "shell.h"
#include <time.h>

char	*get_buf_line(char **line, int *size, int increase)
{
	char	*new;

	struct winsize dimensions;

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
	struct termios	tty;
	t_term			*pos;
	tcgetattr(STDIN_FILENO, old_tty);
	tcgetattr(STDIN_FILENO, &tty);
	tty.c_lflag &= ~(ECHO | ICANON);
//	tty.c_lflag &= ~(ECHO | IEXTEN | ISIG);
	tty.c_cc[VMIN] = 1;
	tty.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &tty);
	pos = create_new_io_struct();
	ft_putstr_size("shelp$>", 7);
	return (pos);
}

void	print_value_into_file(long long key, int x, int y)
{
	FILE *fptr;
	fptr = fopen("input_log", "aw");
	if (key == '\n')
	{
			fprintf(fptr, "%s\n", "--------------------------------");
	} else {
	fprintf(fptr, "%lld: key, %d:x, %d:y", key, x, y);
	if (ft_ischar(key))
		fprintf(fptr, ";%c - char key", (char)key);
	fprintf(fptr, "%c", '\n');
	}
}
/*
int msleep(long msec)
{
    struct timespec ts;
    int res;

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res);

    return res;
}
*/

__attribute__((noinline))long long	incapsulate_read(void)
{
	long long key;
	key = 0;
	read(STDIN_FILENO, &key, sizeof(key));
	return (key);
}

char	*get_input(t_yank *buffer)
{
	ssize_t		red;
	struct termios	old_tty;
	long long	key;
	t_term	*pos;

	ft_bzero(&old_tty, sizeof(struct termios));
	pos = init_prompt(&old_tty);
	key = 0;
	red = 0;
//	coordinates(&pos->y, &pos->x); //winsize
	while (1)
	{
			if (!pos->new)
				pos->new = get_buf_line(&pos->new, &pos->buf_size, 20);
			if (pos->index + 2 >= pos->buf_size)
				pos->new = get_buf_line(&pos->new, &pos->buf_size, 20);
			key = incapsulate_read(); //remake into a char *?
			print_value_into_file(key, pos->x, pos->y);
			red = (read_key(key, pos, old_tty, buffer));
			if (red == -1)
				break ;
			else if (red == -2)
				return (NULL);
			red = 0;
			key = 0;
			draw_cursor_line(pos);
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &old_tty);
	return (pos->new);
}