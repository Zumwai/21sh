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


static long long	incapsulate_read(void)
{
	long long key;
	ssize_t	ret;
	ret = 0;
	key = 0;
	while (1)
	{
		ret = 0;
		ret = read(STDIN_FILENO, &key, sizeof(key));
		//printf("%lld\n", key);
		if (ret == -1)
			handle_exit_errors("read ERRNO");
		else
			break ;
	}
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
	long long	key;
	int tmp;
	t_term	*pos;
    struct winsize dimensions;

	tcsetattr(STDIN_FILENO, TCSANOW, &(buffer)->work);
	pos = create_new_io_struct(NULL);
	ft_putstr_size("shelp$>", 7);
	pos->x += 7;
	key = 0;
	red = 0;
	buffer->current = pos;
	buffer->saved = NULL;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &dimensions);
    g_sad->win_x = dimensions.ws_col;
    g_sad->win_y = dimensions.ws_col;
	while (1)
	{
			key = incapsulate_read();
			red = (read_key(key, buffer->current, buffer->old, buffer, env));
		//	printf("%lld\n", key);
			if (red == DEFAULT || red == -5 || red == -1)
				break ;
			if (red == -2) {
				free_input_line(&buffer->current);
						//free_input_line(&buffer->current);
				ft_putchar('\n');
				break ;
			}
			if (red == -6) {
				buffer->current->main->state ^= (FAILED);
				break ;
			}
			if (red == HIST_UP || red == HIST_D) {
				int tmp = buffer->current->y;
				envoke_history(buffer, red);
				recalc_y(buffer->current, tmp);
			}
			red = 0;
			key = 0;
			display_input(buffer->current, 0);
	}
	if (red == 0)
		buffer->current->main->state = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &(buffer)->old);
	return (buffer->current);
}

char	*handle_input_stream(t_yank *buffer, t_env **env, int *fail)
{
	char	*line;

	line =  NULL;
	buffer->current = get_input(buffer, env);
	if (buffer->current) {
		if ((buffer->current->main->state & FAILED)) {
			(*fail) |= (FAILED);
		}
		line = ft_strdup(buffer->current->main->line);
		buffer->current->main->size = ft_strlen(line);
		//buffer->current->main->state &= ~(FAILED);
		buffer->history = save_history(buffer);
		free_input_line(&buffer->current);
		if (buffer->saved)
			free_input_line(&buffer->saved);
	}
	return (line);
}