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

t_history	*save_history(t_yank *buffer)
{
	t_history	*temp;

	temp = buffer->history;
	if (!buffer->current || !buffer->current->new || buffer->current->new[0] == 0)
	{
	//	free_input_data(&(*buffer)->current);
		return (temp);
	}
	temp = push_history(&(temp), &buffer->hist_ptr);
	temp->line = copy_input_struct(buffer->current);
	buffer->hist_ptr = NULL;
	return (temp);
}
