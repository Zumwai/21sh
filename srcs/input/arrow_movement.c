#include "sh.h"

void move_left(t_term *pos)
{
	if ((pos->index + pos->delta_x) != 0)
		pos->delta_x--;
	else
		tputs(tgetstr("bl", NULL), 1, putchar_like);
}

void move_right(t_term *pos)
{
	if	(pos->delta_x != 0)
			pos->delta_x++;
	else
		tputs(tgetstr("bl", NULL), 1, putchar_like);
}

void change_line_down(t_term *pos)
{
	struct winsize dimensions;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);

	int tmp = pos->delta_x + dimensions.ws_col;

	if (tmp > 0)
	{
		tputs(tgetstr("bl", NULL), 1, putchar_like);
		return ;
	}
	else
		pos->delta_x = tmp;
}

void change_line_up(t_term *pos)
{
	struct winsize dimensions;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);

	int tmp = pos->delta_x - dimensions.ws_col;

	tmp = ft_abs(tmp);
	if (tmp > pos->index)
	{
		tputs(tgetstr("bl", NULL), 1, putchar_like);
		return ;
	}
	else
		pos->delta_x = -tmp;
}