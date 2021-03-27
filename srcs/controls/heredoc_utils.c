/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 15:20:13 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 15:25:47 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

extern t_hdoc		*create_new_hdoc(void)
{
	t_hdoc	*new;

	if (!(new = (t_hdoc *)malloc(sizeof(t_hdoc))))
		handle_exit_errors("Malloc returned NULL");
	new->cord = -1;
	new->eot = NULL;
	new->quotes = 0;
	new->used = false;
	new->next = NULL;
	return (new);
}

extern void			save_coord_hdoc(t_norm *norm)
{
	t_hdoc	*curs;

	if ((*norm->del))
	{
		curs = *(norm->del);
		while (curs->next)
			curs = curs->next;
		curs->next = create_new_hdoc();
		curs = curs->next;
	}
	else
	{
		curs = create_new_hdoc();
		(*norm->del) = curs;
	}
	curs->cord = norm->size + norm->i;
}

extern int			set_quotes(char c, int quotes)
{
	if (!quotes)
	{
		if (c == '\'')
			quotes |= (QUOTE);
		else
			quotes |= (D_QUOTE);
	}
	return (quotes);
}
