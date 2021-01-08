#include "shell.h"

static int draw_finale_line(t_term *pos, int remainder, int curr)
{
		ft_putstr(&pos->new[pos->index - remainder]);
		pos->x = remainder + curr;
		return (0);
}

static void set_cursor(t_term *pos)
{
	struct winsize dimensions;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	if (ft_abs(pos->delta_x) > pos->x)
	{
		pos->delta_y--;
		int tmp = ft_abs(pos->delta_x);
		tmp -= pos->x;
		while (tmp >= dimensions.ws_col)
		{
			pos->delta_y--;
			tmp -= dimensions.ws_col;
		}
		pos->x = dimensions.ws_col - tmp;
	}
	else
		pos->x += pos->delta_x;
//	if (pos->y + pos->delta_y > dimensions.ws_row)
//		tputs(tgetstr("sf", NULL), 1, putchar_like);
	if (pos->next && pos->next->new == NULL)
	{
		pos->delta_y++;
		pos->x = 0;
		if (pos->y + pos->delta_y>= dimensions.ws_row){
			tputs(tgetstr("sf", NULL), 1, putchar_like);
		}
	}
	tputs (tgoto (tgetstr("cm", NULL), pos->x, pos->y + pos->delta_y - 1), 1, putchar_like);
}

static int draw_line(t_term *pos, int remainder)
{
	int		printed = 0;
	int		curr = 0;
	struct winsize dimensions;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	curr = 0;
	if (!pos->delta_y)
		curr = pos->prompt;
	if (!pos->y) //very dangerous. Need check for head struct inb4 everything shuts down
		pos->y++;
	if (dimensions.ws_col > remainder + curr)
		return (draw_finale_line(pos,remainder, curr));
	else {
		printed = dimensions.ws_col - curr;
	//	if (pos->y > 0 && pos->y + pos->delta_y > 0)
			ft_putstr_size(&pos->new[pos->index - remainder], printed);
		if (dimensions.ws_row == pos->y + pos->delta_y)
		{
			tputs(tgetstr("sf", NULL), 1, putchar_like);
			pos->y--;
			t_term *curs = pos;
			while (curs->prev)
			{
				curs = curs->prev;
				pos->delta_y++;
				curs->y--;
			}
			pos->delta_y++;
		}
		else if (printed + curr == dimensions.ws_col)
			pos->delta_y += 1;
		pos->x = 0;
		return (remainder - printed);
	}
}

static void set_empty_line(int y)
{
	struct winsize dimensions;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	tputs (tgoto (tgetstr("cm", NULL), 0, y - 1), 1, putchar_like);
	tputs(tgetstr("cb", NULL), 1, putchar_like);
	tputs(tgetstr("cd", NULL), 1, putchar_like);
}

int display_input(t_term *pos, int delta)
{
	int	remainder = pos->index;
//	int		y = 0;
	t_term	temp;

	temp = *pos;
	if (!pos->new)
		return (0);
	if (delta)
		pos->y += delta;
	set_empty_line(pos->y);
	if (!delta)
		ft_putstr_fd("shelp$>", 1);
	while (remainder)
		remainder = (draw_line(pos, remainder));
	set_cursor(pos);
	if (pos->next)
		display_input(pos->next, delta + pos->delta_y + 1);
	pos->delta_y = 0;
//	y = pos->y;
	if (delta)
		pos->y = temp.y;
	return (0);
//	return (temp.y - y);
//	*pos = temp;
}

