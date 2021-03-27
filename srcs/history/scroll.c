/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scroll.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 18:02:07 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 18:04:21 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

t_history		*scroll_history_down(t_yank *buffer)
{
	t_history *ptr;

	ptr = buffer->hist_ptr;
	if (ptr != NULL)
	{
		if (!ptr->prev)
		{
			buffer->hist_ptr = NULL;
			return (NULL);
		}
		else
		{
			if (buffer->hist_ptr->prev)
				buffer->hist_ptr = buffer->hist_ptr->prev;
			return (ptr->prev);
		}
	}
	return (NULL);
}

t_history		*scroll_history_up(t_yank *buffer)
{
	t_history *ptr;

	ptr = buffer->hist_ptr;
	if (ptr == NULL)
	{
		if (buffer->history == NULL)
			return (NULL);
		buffer->hist_ptr = buffer->history;
		ptr = buffer->hist_ptr;
		return (ptr);
	}
	else
	{
		if (ptr->next)
		{
			buffer->hist_ptr = buffer->hist_ptr->next;
			ptr = buffer->hist_ptr;
		}
		return (ptr);
	}
	return (NULL);
}
