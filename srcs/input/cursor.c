/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 18:54:53 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 19:32:53 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int		calc_pos(t_term *pos, int cols)
{
	int diff_y;
	int diff_x;
	int tmp;

	diff_y = 0;
	diff_x = pos->delta_x;
	tmp = ft_abs(pos->delta_x);
	if (tmp > pos->x)
	{
		while (tmp >= pos->x)
		{
			diff_y++;
			tmp -= cols;
			diff_x += cols;
		}
		pos->x += diff_x;
	}
	else
		pos->x += pos->delta_x;
	return (diff_y);
}

static void		correct_again(t_term *pos, int tmp)
{
	t_term *curs;

	curs = pos;
	while (curs)
	{
		curs->y += tmp;
		curs = curs->prev;
	}
}

static void		show_printable(t_term *pos, int tmp)
{
	int		i;
	int		pr;

	i = 0;
	if (pos->store->size)
	{
		while (i <= tmp)
		{
			tputs(tgoto(tgetstr("cm", NULL), 0, 0), 1, putchar_like);
			tputs(tgetstr("sr", NULL), 1, putchar_like);
			pr = pos->store->size - i - 1 - (pos->y + pos->delta_y);
			if (pr > pos->store->size)
				pr = pos->store->size;
			if ((pr == 0 && !pos->prev) || pr < 0)
			{
				ft_putstr("shelp&>");
				ft_putstr(pos->store->arr[0]);
			}
			else
				ft_putstr(pos->store->arr[pr]);
			i++;
		}
	}
}

static void		calc_y_pos(t_term *pos, int diff)
{
	int printed;
	int	tmp;
	int	i;

	printed = 0;
	i = 0;
	if (diff && pos->y <= 0)
	{
		diff = pos->y + pos->delta_y - diff;
		if (diff <= 0)
		{
			tmp = ft_abs(diff);
			printed = pos->y + pos->delta_y;
			if (tmp > pos->store->size)
				tmp = pos->store->size;
			show_printable(pos, tmp);
			correct_again(pos, ft_abs(diff) + 1);
		}
	}
}

void			set_cursor(t_term *pos)
{
	struct winsize	dimensions;
	int				diff_y;
	int				counter;

	counter = 0;
	diff_y = 0;
	ft_memset(&dimensions, 0, sizeof(struct winsize));
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	diff_y = calc_pos(pos, dimensions.ws_col);
	calc_y_pos(pos, diff_y);
	tputs(tgoto(tgetstr("cm", NULL), pos->x,
	pos->y + pos->delta_y - diff_y - 1), 1, putchar_like);
}
