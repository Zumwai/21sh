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

int 	read_key(long long key, t_term *pos, struct termios old, t_yank *buf)
{
			if (key == 27)
				return (key_exit(old, pos, buf));
			else if (key == ENTER)
				return (consult_state(key, pos));
			else if (key == BACKSPACE)
				backspace_char(pos);
			else if (key == DELETE)
				delete_char(pos);
			else if (key >= 32 && key <= 127)
				insert_char(pos, key);
			else if (key == LEFT)
				move_left(pos);
			else if (key == RIGHT)
				move_right(pos);
			else if (key == L_WORD)
				go_prev_word(pos);
			else if (key == R_WORD)
				go_next_word(pos);
			else if (key == START)
				move_home(pos);
			else if (key == END)
				move_end(pos);
			else if (key == L_UP )
				change_line_up(pos);
			else if (key == L_DOWN)
				change_line_down(pos);
			else if (key == R_CUT)
				cut_after(pos, buf);
			else if (key == L_CUT)
				cut_before(pos, buf);
			else if (key == YANK)
				yank_buffer(pos, buf);
			else if (key == W_CUT)
				cut_word(pos, buf);
			else if (key == CLEAR)
				ft_clear(pos);
			return (0);
}