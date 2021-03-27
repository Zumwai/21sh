/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_struct.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 18:36:20 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 18:36:27 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"


static void	copy_data_term(t_term **curs, t_term *old)
{
	t_term *new;

	new = (*curs);
	new->y = old->y;
	new->x = old->x;
	new->delta_x = old->delta_x;
	new->delta_y = old->delta_y;
	new->index = old->index;
	new->buf_size = old->buf_size;
	new->next = NULL;
	new->main = NULL;
	new->store = NULL;
}

static void cpy_struct(t_term *curs, t_term *current, t_actual *clone)
{
	if (current->new)
		curs->new = ft_strdup_size(current->new, current->buf_size);
	if (current->store)
	{
		curs->store = (t_scroll *)malloc(sizeof(t_scroll));
		curs->store->arr = NULL;
		curs->store->size = 0;
	}
	if (clone)
		curs->main = clone;
}

void	cycling_hist(t_term *current, t_term *curs, t_actual *clone)
{
	t_term *head;
	t_term	*tmp;

	head = curs;
	tmp = curs;
	while (current)
	{
		copy_data_term(&curs, current);
		if (tmp != head)
			curs->prev = tmp;
		cpy_struct(curs, current, clone);
		current = current->next;
		if (current)
		{
			tmp = curs;
			curs->next = (t_term *)malloc(sizeof(t_term));
			curs->next->prev = curs;
			curs = curs->next;
			curs->prev = tmp;
		}
	}
}