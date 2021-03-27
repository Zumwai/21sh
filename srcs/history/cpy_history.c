/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cpy_history.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 17:58:55 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 18:06:14 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	cut_last_suffix(char *new, t_actual *main)
{
	int	m_len;
	int	s_len;

	m_len = ft_strlen(main->line);
	s_len = ft_strlen(new);
	if (m_len == 0)
		return ;
	while (s_len > 0)
	{
		main->line[m_len - s_len] = 0;
		s_len--;
	}
}

static t_actual	*clone_main_line(t_actual *main)
{
	t_actual	*new;

	new = NULL;
	if (!(new = (t_actual *)malloc(sizeof(t_actual))))
		handle_exit_errors("Malloc returned NULL");
	ft_memset(new, 0, sizeof(t_actual));
	new->line = NULL;
	main->state &= ~(FAILED);
	if (main->line)
		new->line = ft_strdup(main->line);
	new->state = main->state_before;
	new->state_before = main->state_before;
	if (main->hdoc)
		new->hdoc = clone_hdoc(main->hdoc);
	return (new);
}

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

t_term	*cpy_struct(t_term *curs, t_term *current)
{
	if (tmp != head)
		curs->prev = tmp;
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
	current = current->next;
	if (current)
	{
		tmp = curs;
		curs->next = (t_term *)malloc(sizeof(t_term));
		curs = curs->next;
		curs->prev = tmp;
	}
}

t_term	*copy_input_struct(t_term *current, int read)
{
	t_term *head;
	t_term	*curs;
	t_term	*tmp;
	t_actual *clone;

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
		if (current->store)
		{
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
	if (read)
		cut_last_suffix(curs->new, curs->main);
	return (head);
}