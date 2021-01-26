#include "sh.h"

static t_history	*push_history(t_history **history, __attribute__((unused))t_history **ptr)
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

static t_term *copy_input_struct(t_term *current)
{
	t_term *head;
	t_term	*curs;
	t_term	*tmp;

	if (!(curs = (t_term *)malloc(sizeof(t_term))))
		handle_exit_errors("malloc returned NULL");
	head = curs;
	tmp = head;
	curs->prev = NULL;
	while (current)
	{
		ft_memcpy(curs, current, sizeof(t_term));
		if (tmp != head)
			curs->prev = tmp;
		if (current->new)
			curs->new = ft_strdup_size(current->new, current->buf_size);
		if (current->substr)
			curs->substr = ft_strdup_size(current->new, current->buf_size);
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
	if (!buffer->current || !buffer->current->new || buffer->current->new[0] == 0)
		return (temp);
	if (buffer->history)
		if (!ft_strcmp(buffer->current->new, buffer->history->line->new))
			return (temp);
	temp = push_history(&(temp), &buffer->hist_ptr);
	temp->line = copy_input_struct(buffer->current);
	buffer->counter++;
	if (buffer->counter > 50)
	{
		remove_last_command(&buffer->history);
		buffer->counter--;
	}
	buffer->hist_ptr = NULL;
	return (temp);
}

static t_history		*scroll_history_down(t_yank *buffer)
{
	t_history *ptr;

	ptr = buffer->hist_ptr;
	if (ptr != NULL) {
		if (!ptr->prev)
		{
			buffer->hist_ptr = NULL;
			return (NULL);
		}
		else {
			if (buffer->hist_ptr->prev)
				buffer->hist_ptr = buffer->hist_ptr->prev;
			return (ptr->prev);
		}
	}
	return (NULL);
}

static t_history		*scroll_history_up(t_yank *buffer)
{
	t_history *ptr;

	ptr = buffer->hist_ptr;
	if (ptr == NULL)
	{
		if (buffer->history == NULL)
			return (NULL);
		buffer->hist_ptr = buffer->history;
		ptr = buffer->hist_ptr;
		return ptr;
	}
	else {
		if (ptr->next)
		{
			buffer->hist_ptr = buffer->hist_ptr->next;
			ptr = buffer->hist_ptr;
		}
		return ptr;
	}
	return (NULL);
}

void	envoke_history(t_yank *buffer, int key)
{
	t_history	*temp;

	temp = NULL;
	if (key == -4) {
		temp = scroll_history_down(buffer);
	}
	if (key == -3) {
		temp = scroll_history_up(buffer);
	}
	if (temp != NULL) {
		if (buffer->saved == NULL)
			buffer->saved = buffer->current;
		if (buffer->current != buffer->saved) {
			free_input_line(&buffer->current);
			buffer->current = NULL;
		}
		buffer->current = copy_input_struct(buffer->hist_ptr->line);
	}
	if (temp == NULL)
	{
		if (buffer->saved != NULL){
			free_input_line(&buffer->current);
			buffer->current = buffer->saved;
			buffer->saved = NULL;
		}
	}
}