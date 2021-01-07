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
/*
void	envoke_history(t_yank *buffer, int key)
{
	if (buffer->saved == NULL)
		buffer->saved = buffer->current;
	if(key == -3)
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
	if(key == -4)
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
*/

char	*ft_strdup_size(char *old, size_t size)
{
	char	*new;

	if (!(new = ft_strnew(size)))
		handle_exit_errors("Malloc returned NULL");
	ft_strcpy(new,old);
	return (new);
}

t_term *copy_input_struct(t_term *current)
{
	t_term *head;
	t_term	*curs;
	t_term	*tmp;

	curs = (t_term *)malloc(sizeof(t_term));
	head = curs;
	tmp = head;
	curs->prev = NULL;
	while (current)
	{
		ft_memcpy(curs, current, sizeof(t_term));
		if (tmp != head)
			curs->prev = tmp;
		if (current->new)
			curs->new = ft_strdup_size(current->new, current->buf_size);
		if (current->substr)
			curs->substr = ft_strdup_size(current->new, current->buf_size);
		current = current->next;
		if (current)
		{
			curs->next = (t_term *)malloc(sizeof(t_term));
			tmp = curs;
			curs = curs->next;
		}
	}
	return (head);
}

t_history		*scroll_history_down(t_yank *buffer)
{
	t_history *ptr;

	ptr = buffer->hist_ptr;
	if (ptr->prev != NULL) {
		return (ptr);
	}
	return (NULL);
}

t_history		*scroll_history_up(t_yank *buffer)
{
	t_history *ptr;

	ptr = buffer->hist_ptr;
	if (ptr->next != NULL) {
		return (ptr);
	}
	return (NULL);
}

void	envoke_history(t_yank *buffer, int key)
{
	t_history	*temp;

	temp = NULL;
	if (key == -4) {
		temp = scroll_history_down(buffer);
	}
	if (key == -3) {
		temp = scroll_history_up(buffer);
	}
	if (temp != NULL) {
		if (buffer->current != buffer->saved && buffer->saved != NULL) {
			free_input_line(buffer->current);
		}
		if (buffer->saved == NULL)
			buffer->saved = buffer->current;
		buffer->current = copy_input_struct(buffer->hist_ptr->line);
	}
	if (temp == NULL)
	{
		if (buffer->saved != NULL)
			buffer->current = buffer->saved;
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
	buffer->saved = NULL;
	while (1)
	{
			key = incapsulate_read();
//			print_value_into_file(key, buffer->current->x, buffer->current->y);
			red = (read_key(key, buffer->current, old_tty, buffer));
			if (red == -1 || red == -2) 
				break ;
			if (red == -3 || red == -4)
				envoke_history(buffer, red);
			red = 0;
			key = 0;
			display_input(buffer->current, 0);
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &old_tty);
	return (buffer->current);
}