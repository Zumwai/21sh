#include "sh.h"

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

static void set_cursor(t_term *pos)
{
	struct winsize dimensions;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	calc_x_pos(pos, dimensions.ws_col);

	if (pos->next && pos->next->new == NULL)
	{
		pos->delta_y++;
		pos->x = 0;
		//pos->y--;
		if (pos->y + pos->delta_y>= dimensions.ws_row){
			t_term *curs = pos->prev;
			pos->y--;
			while (curs)
			{
				curs->y--;
				curs = curs->prev;
			}
			tputs(tgetstr("sf", NULL), 1, putchar_like);
		}
	}
	tputs (tgoto (tgetstr("cm", NULL), pos->x, pos->y + pos->delta_y - 1), 1, putchar_like);
}

static void	handle_last_line(t_term *pos, int rows) 
{
	if (rows <= pos->y + pos->delta_y)
	{
		tputs(tgetstr("sf", NULL), 1, putchar_like);
		pos->y--;
		
		t_term *curs = pos;
		curs = curs->prev;
	
		while (curs)
		{
			curs->delta_y++;
			//curs->y--;
			curs = curs->prev;
		}
		
		pos->delta_y++;
	}
	else {
		pos->delta_y += 1;
	}
	pos->x = 0;
}

static int draw_line(t_term *pos, int remainder)
{
	int		printed = 0;
	int		curr = 0;
	struct winsize dimensions;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	curr = 0;
	if (remainder == pos->index)
		curr = pos->prompt;
	if (!pos->y)
		pos->y++;
	if (dimensions.ws_col > remainder + curr)
		return (draw_finale_line(pos,remainder, curr));
	else {
		printed = dimensions.ws_col - curr;
		ft_putstr_size(&pos->new[pos->index - remainder], printed);
		handle_last_line(pos, dimensions.ws_row);
		pos->x = 0;
		return (remainder - printed);
	}
}

static void set_empty_line(int y)
{
	tputs (tgoto (tgetstr("cm", NULL), 0, y - 1), 1, putchar_like);
	tputs(tgetstr("cb", NULL), 1, putchar_like);
	tputs(tgetstr("cd", NULL), 1, putchar_like);
}

int display_input(t_term *pos, int delta)
{
	int	remainder = pos->index;
	t_term	temp;

	temp = *pos;
	if (!pos->new)
		return (0);
	//if (pos->prev)
//		delta++;
	if (delta)
		pos->y += delta;
	set_empty_line(pos->y);
	if (!pos->prev)
		ft_putstr_fd("shelp$>", 1);
	while (remainder)
		remainder = draw_line(pos, remainder);
	set_cursor(pos);
	if (pos->next)
		display_input(pos->next, delta + pos->delta_y);
	pos->delta_y = 0;
	if (delta)
		pos->y-= delta;
	return (0);
}