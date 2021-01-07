#include "shell.h"

void	ft_clear(t_term *pos)
{
		ft_printf("%s", "\033[2J");
		tputs (tgoto (tgetstr("cm", NULL), 0, 0), 1, putchar_like);
		pos->y = 0;
}

int		determine_state(char *line, int prev, t_term *pos)
{
	size_t	index = 0;
	int		state;

	state = prev;
	while (line[index])
	{
		if (line[index] == '\'')
		{
			if (state == DEFAULT)
				state = QUOTES;
			else if (state == QUOTES)
				state = DEFAULT;
		}
		if (line[index] == '\"')
		{
			if (state == DEFAULT)
				state = DOUBLE_QUOTES;
			else if (state == DOUBLE_QUOTES)
				state = DEFAULT;
		}
		if (line[index] == '<' && line[index + 1] == '<')
		{
			if (state == DEFAULT)
			{
				pos->heredoc = index + 1;
				state = HEREDOC;
				return (state);
			}
		}
		index++;
	}
	return (state);
}

char	*get_heredoc_ptr(int heredoc, char *new, int index)
{
	char	*substr = NULL;

	while (new[heredoc] && new[heredoc] == ' ')
		heredoc++;
	if (heredoc != index - 1)
		substr = ft_strsub(new, heredoc + 1, index - heredoc);
	return (substr); 
}

int		consult_state(__attribute((unused))long long key, __attribute((unused))t_term *pos)
{
	pos->state = determine_state(pos->new, pos->state, pos);
	if (pos->state == DEFAULT)
	{
		ft_putchar_fd('\n', STDIN_FILENO);
		return (-1);
	}
	else if (pos->state == QUOTES || pos->state == DOUBLE_QUOTES || pos->state == HEREDOC)
	{
		pos->next = create_new_io_struct();
		pos->next->y = pos->y;
		pos->next->new = NULL;
		pos->next->x = 0;
		pos->next->delta_x = 0;
		pos->next->prompt = 0;
		pos->next->state = pos->state;
		pos->next->prev = pos;
		if (pos->state == HEREDOC)
		{
			pos->next->state = POST_DOC;
			pos->substr = get_heredoc_ptr(pos->heredoc, pos->new, pos->index);
			if (pos->substr == NULL)
				return (handle_return_error(-2, "syntax error near unexpected token `newline"));
		}
	}
	else if (pos->state == POST_DOC)
	{
		t_term	*curs;
		curs = pos;
		while (curs->state != HEREDOC)
			curs = curs->prev;
		if (pos->new && !ft_strcmp(pos->new, curs->substr))
			return (-1);
		else
		{
			pos->next = create_new_io_struct();
			pos->next->y = pos->y;
			pos->next->new = NULL;
			pos->next->x = 0;
			pos->next->delta_x = 0;
			pos->next->prompt = 0;
			pos->next->state = pos->state;
			pos->next->prev = pos;
			return (1);
		}
		
	}
	return (1);
}

/*
Needs an update for sublines
*/
void	move_home(t_term *pos)
{
	pos->delta_x = -pos->index;
}

void	move_end(t_term *pos)
{
	pos->delta_x = 0;
}

static t_term *get_last_pos(t_term *pos)
{
	t_term *curs;

	curs = pos;
	while (curs->next != NULL)
		curs = curs->next;
	return (curs);
}

static int	ft_history_up(void)
{
	return (-3);
}

static int	ft_history_down(void)
{
	return (-4);
}

int 	read_key(long long key, t_term *pos, struct termios old, t_yank *buf)
{
	t_term	*curs;

	curs = get_last_pos(pos);
	if (!curs->new)
		curs->new = get_buf_line(&curs->new, &curs->buf_size, 20);
	if (curs->index + 2 >= curs->buf_size)
		curs->new = get_buf_line(&curs->new, &curs->buf_size, 20);
	if (key == 27)
		return (key_exit(old, pos, buf));
	else if (key == ENTER)
		return (consult_state(key, curs));
	else if (key == BACKSPACE)
		backspace_char(curs);
	else if (key == DELETE)
		delete_char(curs);
	else if (key >= 32 && key <= 127)
		insert_char(curs, key);
	else if (key == LEFT)
		move_left(curs);
	else if (key == RIGHT)
		move_right(curs);
	else if (key == L_WORD)
		go_prev_word(curs);
	else if (key == R_WORD)
		go_next_word(curs);
	else if (key == START)
		move_home(curs);
	else if (key == END)
		move_end(curs);
	else if (key == L_UP )
		change_line_up(curs);
	else if (key == L_DOWN)
		change_line_down(curs);
	else if (key == R_CUT)
		cut_after(curs, buf);
	else if (key == L_CUT)
		cut_before(curs, buf);
	else if (key == COPY_W)
		copy_word(curs, buf);
	else if (key == YANK)
		yank_buffer(curs, buf);
	else if (key == W_CUT)
		cut_word(curs, buf);
	else if (key == CLEAR)
			ft_clear(curs);
	else if (key == HISTORY_UP)
			return (ft_history_up());
	else if (key == HISTORY_DOWN)
			return (ft_history_down());
	return (0);
}