/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cpy_history.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 17:58:55 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 18:40:40 by aophion          ###   ########.fr       */
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

t_term	*copy_input_struct(t_term *current, int read)
{
	t_term		*curs;
	t_term		*head;
	t_actual	*clone;

	if (!(curs = (t_term *)malloc(sizeof(t_term))))
		handle_exit_errors("malloc returned NULL");
	curs->prev = NULL;
	head = curs;
	clone = NULL;
	if (current->main)
		clone = clone_main_line(current->main);
	curs->main = clone;
	cycling_hist(current, curs, clone);
	if (read)
		cut_last_suffix(curs->new, curs->main);
	return (head);
}
