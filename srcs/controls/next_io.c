/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   next_io.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 16:00:00 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 16:00:20 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static t_term *create_next_io(t_actual **line, int y, t_term *curs)
{
	t_term *input;

	input = create_new_io_struct(line);
	input->y = y;
	if (*line)
		input->main = *line;
	input->prev = curs;
	return input;
}

static int		check_hdoc_eot(t_hdoc **head, char *sub)
{
	t_hdoc	*curs;

	curs = *head;
	if (!curs)
		return (1);
	while (curs)
	{
		if (curs->used == false)
		{
			if (ft_strequ(sub, curs->eot))
			{
				curs->used = true;
				if (!curs->next)
					return 0;
				return (1);
			}
			break ;
		}
		curs = curs->next;
	}
	return (1);
}

int		determine_next_io_step(t_term *curs, int ret, t_norm *norm)
{
	int		res;

	res = 1;
	if (curs->main->state & (FAILED))
		return (-1);
	if (!curs->main->state)
		return (0);
	if ((curs->main->state & HEREDOC))
	{
		res = update_hdoc_list(&curs->main->hdoc, curs->main->line);
		if (!res)
			res = check_hdoc_eot(&curs->main->hdoc, curs->new);
		if (res)
			curs->next = create_next_io(&curs->main, curs->y, curs);
	}
	else
		curs->next = create_next_io(&curs->main, curs->y, curs);
	return (res);
}