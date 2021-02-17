#include "sh.h"

static void	ft_clear(t_term *pos)
{
		ft_printf("%s", "\033[2J");
		tputs (tgoto (tgetstr("cm", NULL), 0, 1), 1, putchar_like);
		pos->y = 1;
}

static char	*get_heredoc_ptr(int heredoc, char *new, int index)
{
	char	*substr = NULL;

	while (new[heredoc] && new[heredoc] == ' ')
		heredoc++;
	if (heredoc != index - 1)
		substr = ft_strsub(new, heredoc + 1, index - heredoc);
	return (substr); 
}

static t_term *create_next_io(int y, int state)
{
	t_term *input;

	input = create_new_io_struct();
	input->y = y;
	input->new = NULL;
	input->x = 0;
	input->delta_x = 0;
	input->prompt = 0;
	input->state = state;
	return input;
}

static int		consult_state(__attribute((unused))long long key, __attribute((unused))t_term *pos)
{
	char *glued = NULL;
	glued = determine_glue(pos->new, pos, ft_strlen(pos->new));
	if (!glued && !pos->glue)
		pos->state = determine_state(pos->new, pos->state, pos);
	/* glue! */
	if (pos->state == DEFAULT)
	{
		ft_putchar_fd('\n', STDIN_FILENO);
		if (!pos->glue || pos->glue == FIN)
			return (-1);
		pos->next = create_next_io(pos->y, pos->state);
		pos->next->prev = pos;
		return 0;
	}
	else if (pos->state == QUOTES || pos->state == DOUBLE_QUOTES || pos->state == HEREDOC)
	{
		pos->next = create_next_io(pos->y, pos->state);
		pos->next->prev = pos;
		if (pos->state == HEREDOC)
		{
			pos->next->state = POST_DOC;
			pos->substr = get_heredoc_ptr(pos->heredoc, pos->new, pos->index);
			if (pos->substr == NULL)
				return (handle_return_error(-1, "syntax error near unexpected token `newline"));
		}
	}
	else if (pos->state == POST_DOC || !pos->glue)
	{
		t_term	*curs;
		curs = pos;
		while (curs->state != HEREDOC)
			curs = curs->prev;
		if (pos->new && !ft_strcmp(pos->new, curs->substr))
			return (-1);
		else
		{
			pos->next = create_next_io(pos->y, pos->state);
			pos->next->prev = pos;
			return (1);
		}
	}
	return (1);
}

/*
Needs an update for sublines
*/
static void	move_home(t_term *pos)
{
	pos->delta_x = -pos->index;
}

static void	move_end(t_term *pos)
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
	return (HIST_UP);
}

static int	ft_history_down(void)
{
	return (HIST_D);
}


int 	read_key(long long key, t_term *pos, struct termios old, t_yank *buf, t_env **env)
{
	t_term	*curs;
	if (key == 27 || (key == 4 && (!pos->new || !pos->new[0])))
		return (key_exit(old, pos, buf));
	curs = get_last_pos(pos);
	if (key == 4 && (!curs->new || !curs->new[0]))
		return -5;
	if (!curs->new)
		curs->new = get_buf_line(&curs->new, &curs->buf_size, 20);
	if (curs->index + 2 >= curs->buf_size)
		curs->new = get_buf_line(&curs->new, &curs->buf_size, 20);
	if (key == ENTER)
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
		yank_buffer(curs, buf->yanked);
	else if (key == W_CUT)
		cut_word(curs, buf);
	else if (key == CLEAR)
			ft_clear(curs);
	else if (key == HISTORY_UP)
			return (ft_history_up());
	else if (key == HISTORY_DOWN)
			return (ft_history_down());
	else if (key == TAB)
		autocomplete(curs, env, buf);
	return (0);
}