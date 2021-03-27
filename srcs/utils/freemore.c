/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freemore.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 19:37:33 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 19:48:47 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void		set_free_null(char **line)
{
	if (*line)
		free(*line);
	*line = NULL;
}

void		free_eot_list(t_hdoc **t)
{
	t_hdoc	*curs;
	t_hdoc	*tmp;

	curs = (*t);
	while (curs)
	{
		tmp = curs->next;
		if (curs->eot)
			set_free_null(&curs->eot);
		free(curs);
		curs = NULL;
		curs = tmp;
	}
	curs = NULL;
	*t = NULL;
}

void		free_main_line(t_actual **main)
{
	t_actual *curs;

	curs = *main;
	if (!(*main))
		return ;
	if ((*main)->hdoc)
		free_eot_list(&(*main)->hdoc);
	if ((*main)->line)
		set_free_null(&(*main)->line);
	(*main)->line = NULL;
	free(*main);
	*main = NULL;
	main = NULL;
}

void		free_input_line(t_term **input)
{
	if (!(*input))
		return ;
	if (!(*input)->prev)
	{
		free_main_line(&(*input)->main);
		(*input)->main = NULL;
	}
	else
		(*input)->main = NULL;
	if ((*input)->new)
		set_free_null(&(*input)->new);
	if ((*input)->store)
	{
		free((*input)->store);
		(*input)->store = NULL;
	}
	if ((*input)->next)
		free_input_line(&(*input)->next);
	if (*input)
		free(*input);
	*input = NULL;
}

extern void	free_history(t_history **history)
{
	t_history *curs;
	t_history *tmp;

	curs = (*history);
	while (curs)
	{
		tmp = curs;
		if (curs->line)
			free_input_line(&curs->line);
		curs = curs->next;
		free(tmp);
		tmp = NULL;
	}
}
