#include "shell.h"

static int draw_finale_line(t_term *pos, int remainder, int curr, char *new)
{
		ft_putstr(&new[pos->index - remainder]);
		pos->x = remainder + curr;
		return (0);
}

static void set_cursor(t_term *pos)
{
	struct winsize dimensions;
	struct winsize tempo;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	tempo = dimensions;
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
	if (pos->y + pos->delta_y >dimensions.ws_row)
		ft_printf("\033[S");
	tputs (tgoto (tgetstr("cm", NULL), pos->x, pos->y + pos->delta_y - 1), 1, putchar_like);
}

static int draw_line(char *new, t_term *pos, int remainder)
{
	int		printed = 0;
	int		curr = 0;
	struct winsize dimensions;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	curr = 0;
	if (!pos->delta_y)
		curr = pos->prompt;
	if (dimensions.ws_col > remainder + curr)
		return (draw_finale_line(pos,remainder, curr, new));
	else {
		printed = dimensions.ws_col - curr;
		ft_putstr_size(&new[pos->index - remainder], printed);
		if (dimensions.ws_row == pos->y + pos->delta_y)
		{
		//	tputs(tgetstr("sf", NULL), 1, putchar_like);
			ft_printf("\033[S");
			pos->y--;
			pos->delta_y++;
		}
		else if (printed + curr == dimensions.ws_col)
			pos->delta_y += 1;
		pos->x = 0;
		return (remainder - printed);
	}
}

void draw_cursor_line(char *new, t_term *pos)
{
	int i = 0;
	int	rem = pos->index;
	int	fin = 0;
	int	tmp = pos->index + pos->prompt;
	struct winsize dimensions;
	t_term		temp = *pos;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	tputs (tgoto (tgetstr("cm", NULL), 0, pos->y - 1), 1, putchar_like);
	tputs(tgetstr("cb", NULL), 1, putchar_like);
	tputs(tgetstr("cd", NULL), 1, putchar_like);
	ft_putstr_fd("shelp$>", 1);
	while (1)
	{
		rem = (draw_line(new, pos, rem));
		if (rem == 0)
			break ;
	}
	set_cursor(pos);
	pos->delta_y = 0;
}

