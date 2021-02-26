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

t_actual	*clone_main_line(t_actual *main)
{
	t_actual	*new;

	new = NULL;

	if (!(new = (t_actual *)malloc(sizeof(t_actual))))
		handle_exit_errors("Malloc returned NULL");
	ft_memset(new, 0, sizeof(t_actual));
	new->line = NULL;
	main->state &= ~(FAILED);
	if(main->line)
		new->line = ft_strdup(main->line);
	new->state = main->state_before;
	new->state_before = main->state_before;
	if (main->hdoc)
		new->hdoc = clone_hdoc(main->hdoc);
	return new;
}

void	cut_last_suffix(char *new, t_actual *main)
{
	int	m_len;
	int	s_len;
	m_len = ft_strlen(main->line);
	s_len = ft_strlen(new);

	//if (m_len > s_len) /* simple check for multiline */
		//s_len++;
	if (m_len == 0) /* case for handling envoking line from history */
		return ;
	while (s_len > 0)
	{
		main->line[m_len - s_len] = 0;
		//if (main->line[m_len - s_len] != '\n')
			s_len--;
	}
}

void	copy_data_term(t_term **curs, t_term *old)
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

static t_term *copy_input_struct(t_term *current, int read)
{
	t_term *head;
	t_term	*curs;
	t_term	*tmp;
	t_actual *clone = NULL;

	if (!(curs = (t_term *)malloc(sizeof(t_term))))
		handle_exit_errors("malloc returned NULL");
	head = curs;
	tmp = head;
	curs->prev = NULL;
	clone = NULL;
	if (current->main)
		clone = clone_main_line(current->main);
	curs->main = clone;
	while (current)
	{
		copy_data_term(&curs, current);
		if (tmp != head)
			curs->prev = tmp;
		if (current->new)
			curs->new = ft_strdup_size(current->new, current->buf_size);
		if (current->store) {
			curs->store = (t_scroll *)malloc(sizeof(t_scroll));
			curs->store->arr = NULL;
			curs->store->size = 0;
		}
		if (clone)
			curs->main = clone;
		current = current->next;
		if (current)
		{
			tmp = curs;
			curs->next = (t_term *)malloc(sizeof(t_term));
			curs = curs->next;
			curs->prev = tmp;
		}
	}
	//curs->next = NULL;
	/* call on history call only */
	if (read)
		cut_last_suffix(curs->new, curs->main);
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
	if (!buffer->current || !buffer->current->main || buffer->current->main->line[0] == 0)
		return (temp);
	if (buffer->history)
		if (!ft_strcmp(buffer->current->main->line, buffer->history->line->main->line)) {
			buffer->hist_ptr = NULL;
			return (temp);
		}
	temp = push_history(&(temp), &buffer->hist_ptr);
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
		buffer->current = copy_input_struct(buffer->hist_ptr->line, 1);
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