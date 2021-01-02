#include "shell.h"

t_history	*push_history(t_history **history, __attribute__((unused))t_history **ptr)
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

t_term *save_command(t_term *current)
{
	t_term *head;
	t_term	*curs;
	t_term	*tmp;

	curs = (t_term *)malloc(sizeof(t_term));
	head = curs;
	tmp = head;
	curs->prev = NULL;
	while (current)
	{
		ft_memcpy(curs, current, sizeof(t_term));
		if (tmp != head)
			curs->prev = tmp;
		if (current->new)
			curs->new = ft_strdup(current->new);
		if (current->substr)
			curs->substr = ft_strdup(current->new);
		current = current->next;
		if (current)
		{
			curs->next = (t_term *)malloc(sizeof(t_term));
			tmp = curs;
			curs = curs->next;
		}
	}
	return (head);
}

t_history	*save_history(t_yank *buffer)
{
	t_history	*temp;

	temp = buffer->history;
	if (!buffer->current)
		return (temp);
	if (!buffer->current->new)
		return (temp);
	if (buffer->current->new[0] == 0)
		return (temp);
	temp = push_history(&(temp), &buffer->hist_ptr);
	temp->line = save_command(buffer->current);
	return (temp);
}