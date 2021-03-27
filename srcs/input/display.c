/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 18:43:11 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 19:29:39 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void		handle_last_line(t_term *pos, int rows, int rem, int print)
{
	if (pos->y + pos->delta_y >= rows)
	{
		if (pos->y > rows)
			pos->y = rows;
		correct_y(pos);
		tputs(tgetstr("sf", NULL), 1, putchar_like);
	}
	if (pos->y + pos->delta_y > 0)
	{
		tputs(tgoto(tgetstr("cm", NULL), 0,
		pos->y + pos->delta_y), 1, putchar_like);
	}
	else
		tputs(tgoto(tgetstr("cm", NULL), 0, 1), 1, putchar_like);
	pos->x = 0;
	pos->delta_y++;
}

void			append_arr(t_term *pos, char *line, int len)
{
	char	**new;
	int		i;

	i = 0;
	pos->store->size++;
	if (!(new = (char **)malloc(sizeof(char *) * (pos->store->size + 1))))
		handle_exit_errors("Malloc returned NULL");
	ft_memset(new, 0, sizeof(char *) * (pos->store->size + 1));
	if (pos->store->arr)
	{
		while (i < pos->store->size)
		{
			new[i] = pos->store->arr[i];
			i++;
		}
	}
	free(pos->store->arr);
	pos->store->arr = NULL;
	if (i > 0)
	{
		pos->store->size = i;
		i--;
	}
	new[i] = ft_strndup(line, len);
	pos->store->arr = new;
}

static int		draw_line(t_term *pos, int remainder)
{
	struct winsize	dimensions;
	int				print;
	int				curr;

	curr = 0;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &dimensions);
	if (remainder == pos->index && !pos->prev)
		curr = 7;
	print = dimensions.ws_col - curr;
	if (print > remainder)
		print = remainder;
	if (pos->y + pos->delta_y > 0)
	{
		ft_putstr_size(&pos->new[pos->index - remainder], print);
		pos->x = print + curr;
	}
	append_arr(pos, &pos->new[pos->index - remainder], print);
	if ((pos->next && remainder == print)
	|| (print + curr == dimensions.ws_col))
		handle_last_line(pos, dimensions.ws_row, remainder, print + curr);
	if ((remainder - print) == 0)
		return (-1);
	return (remainder - print);
}

static void		set_empty_line(t_term *pos, int y, int prev)
{
	if (pos->y + pos->delta_y > 0)
	{
		tputs(tgetstr("cb", NULL), 1, putchar_like);
		tputs(tgetstr("cd", NULL), 1, putchar_like);
		tputs(tgoto(tgetstr("cm", NULL), 0,
		y + pos->delta_y - 1), 1, putchar_like);
		if (!prev)
			ft_putstr_fd("shelp$>", 1);
	}
	else
	{
		tputs(tgoto(tgetstr("cm", NULL), 0, 0), 1, putchar_like);
	}
}

int				display_input(t_term *pos, int delta)
{
	int		remainder;
	t_term	curr;
	int		tmp_y;

	if (!pos)
		return (0);
	curr = *pos;
	remainder = curr.index;
	if (curr.prev)
		curr.delta_y += delta;
	set_empty_line(&curr, curr.y, !!curr.prev);
	while (remainder >= 0)
		remainder = draw_line(&curr, remainder);
	if (!curr.next)
		tputs(tgetstr("cd", NULL), 1, putchar_like);
	set_cursor(&curr);
	if (curr.next)
	{
		if ((tmp_y = display_input(curr.next, curr.delta_y)))
			curr.y = tmp_y;
	}
	free_storage(&curr);
	*pos = curr;
	return (curr.y);
}
