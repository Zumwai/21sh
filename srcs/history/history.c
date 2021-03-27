/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 17:53:57 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 18:04:00 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static t_history	*push_history(t_history **history)
{
	t_history *new;

	if (!(new = (t_history *)malloc(sizeof(t_history))))
		handle_exit_errors("Malloc returned NULL");
	if (!(*history))
	{
		(*history) = (t_history *)new;
		new->next = NULL;
		new->prev = NULL;
	}
	else
	{
		new->next = (*history);
		new->prev = (*history)->prev;
		(*history)->prev = new;
		(*history) = new;
	}
	return (new);
}

void	remove_last_command(t_history **head)
{
	t_history *curs;

	curs = *head;
	while (curs->next)
		curs = curs->next;
	curs->prev->next = NULL;
	free_input_line(&curs->line);
	free(curs);
	curs = NULL;
}

t_history	*save_history(t_yank *buffer)
{
	t_history	*temp;

	temp = buffer->history;
	if (!buffer->current || !buffer->current->main
	|| !buffer->current->main->line || buffer->current->main->line[0] == 0)
		return (temp);
	if (buffer->history)
		if (!ft_strcmp(buffer->current->main->line,
		buffer->history->line->main->line))
		{
			buffer->hist_ptr = NULL;
			return (temp);
		}
	temp = push_history(&(temp));
	temp->line = copy_input_struct(buffer->current, DEFAULT);
	buffer->counter++;
	if (buffer->counter > 50)
	{
		remove_last_command(&buffer->history);
		buffer->counter--;
	}
	buffer->hist_ptr = NULL;
	return (temp);
}

void	envoke_history(t_yank *buffer, int key)
{
	t_history	*temp;

	temp = NULL;
	if (key == -4)
		temp = scroll_history_down(buffer);
	if (key == -3)
		temp = scroll_history_up(buffer);
	if (temp != NULL)
	{
		if (buffer->saved == NULL)
			buffer->saved = buffer->current;
		if (buffer->current != buffer->saved)
		{
			free_input_line(&buffer->current);
			buffer->current = NULL;
		}
		buffer->current = copy_input_struct(buffer->hist_ptr->line, 1);
	}
	if (temp == NULL && buffer->saved != NULL)
	{
		free_input_line(&buffer->current);
		buffer->current = buffer->saved;
		buffer->saved = NULL;
	}
}
