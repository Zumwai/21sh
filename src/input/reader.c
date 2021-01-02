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
	return (pos);
}

void	print_value_into_file(long long key, int x, int y)
{
	FILE *fptr;
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

void	envoke_history(t_yank *buffer, int key)
{
	if (buffer->saved == NULL)
		buffer->saved = buffer->current;
	if(key == HISTORY_UP)
	{
		if (buffer->history)
		{
			if (buffer->hist_ptr)
			{
				if (buffer->hist_ptr->next)
					buffer->hist_ptr = buffer->hist_ptr->next;
			}
			else
				buffer->hist_ptr = buffer->history;
			buffer->current = buffer->hist_ptr->line;
			buffer->current->y = buffer->saved->y;
		}
	}
	if(key == HISTORY_DOWN)
	{
		if (buffer->hist_ptr && buffer->hist_ptr->prev)
		{
			buffer->hist_ptr = buffer->hist_ptr->prev;
			buffer->current = buffer->hist_ptr->line;
			buffer->current->y = buffer->saved->y;
		}
		else if (buffer->hist_ptr && !buffer->hist_ptr->prev)
		{
			buffer->current = buffer->saved;
			buffer->hist_ptr = NULL;
		}
	}
}

t_term *get_input(t_yank *buffer)
{
	ssize_t		red;
	struct termios	old_tty;
	long long	key;
	t_term	*pos;
//	t_term	*head;

	ft_bzero(&old_tty, sizeof(struct termios));
	pos = init_prompt(&old_tty);
	ft_putstr_size("shelp$>", 7);
//	head = pos;
	key = 0;
	red = 0;
	buffer->current = pos;
	buffer->saved = pos;
	while (1)
	{
			key = incapsulate_read();
			print_value_into_file(key, buffer->current->x, buffer->current->y);
			red = (read_key(key, buffer->current, old_tty, buffer));
			if (red == -1 || red == -2) 
				break ;
			if (red == HISTORY_UP || red == HISTORY_DOWN)
				envoke_history(buffer, red);
			red = 0;
			key = 0;
			display_input(buffer->current, 0);
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &old_tty);
	return (buffer->current);
}