#include "shell.h"

void	ft_clear(t_term *pos)
{
		ft_printf("%s", "\033[2J");
		tputs (tgoto (tgetstr("cm", NULL), 0, 0), 1, putchar_like);
		pos->y = 0;
}

int		consult_state(__attribute((unused))long long key, __attribute((unused))t_term *pos)
{
	if (!pos->state)
	{
		ft_putchar_fd('\n', STDIN_FILENO);
		return (-1);
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


t_term *get_last_pos(t_term *pos)
{
	t_term *curs;

	curs = pos;
	while (curs->next)
		curs = curs->next;
	return (curs);
}

int 	read_key(long long key, t_term *pos, struct termios old, t_yank *buf)
{
	t_term	*curs;

	curs = get_last_pos(pos);
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
	else if (key == YANK)
		yank_buffer(curs, buf);
	else if (key == W_CUT)
		cut_word(curs, buf);
	else if (key == CLEAR)
			ft_clear(curs);
	return (0);
}