#include "sh.h"

static int	calc_pos(t_term *pos, int cols)
{
	int	diff_y;

	diff_y = 0;
	int diff_x = pos->delta_x;
	int tmp = ft_abs(pos->delta_x);

	if (tmp > pos->x)
	{
		while (tmp >= pos->x)
		{
			diff_y++;
			tmp -= cols;
			diff_x += cols;
		}
		pos->x += diff_x;
	} else 
		pos->x +=pos->delta_x;
	return diff_y;
}

static void calc_y_pos(t_term *pos, int diff)
{
	if (pos->y <= 0)
	{
		diff = pos->y + pos->delta_y - diff;
		if (diff <= 0)
		{
			int tmp = diff;
			while (tmp <= 0) {
				tputs (tgoto (tgetstr("cm", NULL), 0, 0), 1, putchar_like);
				tputs(tgetstr("sr", NULL), 1, putchar_like);
				tmp++;
			}
			tmp = diff;
			t_term *curs;
			curs = pos;
			while (curs){
				curs->y += tmp;
				curs = curs->prev;
			}
		}
	}
}

static void set_cursor(t_term *pos)
{
	struct winsize dimensions;
	int		diff_y;
	int		counter = 0;
	diff_y = 0;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	diff_y = calc_pos(pos, dimensions.ws_col);
	calc_y_pos(pos, diff_y);
	/*
	if (ft_abs(pos->y) - ft_abs(diff_y) + counter < 0)
	{
		tputs(tgetstr("sf", NULL), 1, putchar_like);
		counter++;
	}
	*/
	//tputs(tgetstr("cd", NULL), 1, putchar_like);
	tputs (tgoto (tgetstr("cm", NULL), pos->x, pos->y + pos->delta_y - diff_y - 1), 1, putchar_like);

}

static void	correct_y(t_term *pos)
{
	t_term *curs = pos;

	while (curs)
	{
		curs->y--;
		curs = curs->prev;
	}
}
static void	handle_last_line(t_term *pos, int rows, int cols, int rem, int print) 
{
	/* Scroll down and correct starting y in previous lines */
	if (pos->y + pos->delta_y >= rows) {
		if (pos->y > rows)
			pos->y = rows;
		correct_y(pos);
		tputs(tgetstr("sf", NULL), 1, putchar_like);
	}
	tputs(tgoto (tgetstr("cm", NULL), 0, pos->y + pos->delta_y), 1, putchar_like);
	pos->delta_y++;
	pos->x = 0;
}

static int draw_line(t_term *pos, int remainder)
{
	int	print = 0;
	int	curr = 0;
	struct winsize dimensions;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &dimensions);
	/* Calculate max possible character in this line */
	if (remainder == pos->index && !pos->prev)
		curr = pos->prompt;
	print = dimensions.ws_col - curr;
	/* Correct if line smaller than available space */
	if (print > remainder)
		print = remainder;

	/* Dont print anything beyond y < 0, just skip */
	if (pos->y + pos->delta_y > 0) {
		ft_putstr_size(&pos->new[pos->index - remainder], print);
		pos->x = print + curr;
	}
	/* Calculate diff and y for current printed part */
	if ((pos->next && remainder == print) || (print + curr == dimensions.ws_col))
		handle_last_line(pos, dimensions.ws_row, dimensions.ws_col, remainder, print + curr);
	return (remainder - print);
}

static void set_empty_line(t_term *pos, int y, int prev)
{
	/*skip clearing if y < 0 */
	if (pos->y + pos->delta_y > 0) {
		tputs (tgoto (tgetstr("cm", NULL), 0, y - 1), 1, putchar_like);
		tputs(tgetstr("cb", NULL), 1, putchar_like);
		tputs(tgetstr("cd", NULL), 1, putchar_like);
		if (!prev)
			ft_putstr_fd("shelp$>", 1);
	} else {
		tputs (tgoto (tgetstr("cm", NULL), 0, 0), 1, putchar_like);
	}
}

int display_input(t_term *pos, int delta)
{
	t_term	*curs = pos;
	int	remainder;

	if (!pos)
		return 0;
	remainder = pos->index;
	if (pos->prev)
		pos->y += delta;
	set_empty_line(pos, pos->y, !!pos->prev);
	while (remainder)
		remainder = draw_line(pos, remainder);
	set_cursor(pos);
	if (curs) {
		display_input(curs->next, pos->delta_y + delta);
	}
	pos->y -= delta;
	pos->delta_y = 0;
	return (0);
}