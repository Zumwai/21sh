#include "sh.h"
/*
static int draw_finale_line(t_term *pos, int remainder, int curr)
{
		ft_putstr(&pos->new[pos->index - remainder]);
		pos->x = remainder + curr;
		return (0);
}

static void	calc_x_pos(t_term *pos, int cols)
{
	int		tmp;

	if (ft_abs(pos->delta_x) > pos->x)
	{
		pos->delta_y--;
		tmp -= pos->x;
		while (tmp >= cols)
		{
			pos->delta_y--;
			tmp -= cols;
		}
		pos->x = cols - tmp;
	}
	else
		pos->x += pos->delta_x;
}

static void calc_y_pos(t_term *pos, int rows)
{
	if (pos->next && pos->next->new == NULL)
	{
		pos->delta_y++;
		pos->x = 0;
		pos->y--;
		if (pos->y + pos->delta_y>= rows){
			t_term *curs = pos->prev;
			//pos->y--;
			while (curs)
			{
				curs->y--;
				//tputs(tgetstr("sf", NULL), 1, putchar_like);
				curs->delta_y ++;
				curs = curs->prev;
			}
			tputs(tgetstr("sf", NULL), 1, putchar_like);
		}
	}
}


static void set_cursor(t_term *pos)
{
	struct winsize dimensions;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	calc_x_pos(pos, dimensions.ws_col);
	calc_y_pos(pos, dimensions.ws_row);
	tputs (tgoto (tgetstr("cm", NULL), pos->x, pos->y + pos->delta_y - 1), 1, putchar_like);
}
*/

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
			diff_y--;
			tmp -= cols;
			diff_x += cols;
		}
		pos->x += diff_x;
	} else 
		pos->x +=pos->delta_x;
	return diff_y;
}

static void calc_y_pos(t_term *pos, int rows)
{

}

static void set_cursor(t_term *pos)
{
	struct winsize dimensions;
	int		diff_y;
	diff_y = 0;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	diff_y = calc_pos(pos, dimensions.ws_col);

	tputs (tgoto (tgetstr("cm", NULL), pos->x, pos->y + pos->delta_y + diff_y - 1), 1, putchar_like);
	calc_y_pos(pos, dimensions.ws_row);
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
	if (pos->y + pos->delta_y >= rows) {
		if (pos->y > rows)
			pos->y = rows;
		correct_y(pos);
		tputs(tgetstr("sf", NULL), 1, putchar_like);
		//pos->delta_y++;
	}
	tputs(tgoto (tgetstr("cm", NULL), 0, pos->y + pos->delta_y), 1, putchar_like);
	pos->delta_y++;
	pos->x = 0;
	//tputs(tgetstr("sf", NULL), 1, putchar_like);
}

static int draw_line(t_term *pos, int remainder)
{
	int	print = 0;
	int	curr = 0;
	struct winsize dimensions;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &dimensions);

	if (remainder == pos->index && !pos->prev)
		curr = pos->prompt;
	print = dimensions.ws_col - curr;
	if (print > remainder)
		print = remainder;
	ft_putstr_size(&pos->new[pos->index - remainder], print);
	pos->x = print + curr;
	if ((pos->next && remainder == print) || (print + curr == dimensions.ws_col))
		handle_last_line(pos, dimensions.ws_row, dimensions.ws_col, remainder, print + curr);

	return (remainder - print);
}

static void set_empty_line(int y, int prev)
{
	tputs (tgoto (tgetstr("cm", NULL), 0, y - 1), 1, putchar_like);
	tputs(tgetstr("cb", NULL), 1, putchar_like);
	tputs(tgetstr("cd", NULL), 1, putchar_like);
	if (!prev)
		ft_putstr_fd("shelp$>", 1);
}

int display_input(t_term *pos, int delta)
{
	t_term	*curs = pos;
	int	remainder;

	if (!pos)
		return 0;
	remainder = pos->index;
	if (pos->prev && pos->y <= pos->prev->y)
		pos->y += delta;
	set_empty_line(pos->y, !!pos->prev);
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