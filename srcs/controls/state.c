/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 16:01:50 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 16:35:21 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void		modify_suffix(char *new, int state, int size)
{
	if ((state & GLUE))
	{
		new[size--] = 0;
		new[size--] = 0;
	}
	else if ((state & HEREDOC) || (state & QUOTE) || (state & D_QUOTE))
	{
		size--;
		new[size - 1] = '\n';
		new[size] = 0;
	}
}

static void		append_main_line(t_actual *main, char *sub, int state)
{
	char	*new;
	int		size;

	size = 0;
	new = NULL;
	if (sub)
		size += ft_strlen(sub) + 1;
	if (main->line)
		size += ft_strlen(main->line);
	if (size == 1)
		size++;
	if (!(state & GLUE) && ((state & HEREDOC)
	|| (state & QUOTE) || (state & D_QUOTE)))
		size++;
	if (!(new = ft_strnew(size + 1)))
		handle_exit_errors("Malloc returned NULL");
	if (main->line)
		ft_strcat(new, main->line);
	ft_strcat(new, sub);
	modify_suffix(new, state, size);
	set_free_null(&main->line);
	main->size = ft_strlen(new);
	main->line = new;
	main->line[size] = 0;
}

static	t_norm	fill_norm(t_term *curs)
{
	t_norm	norm;

	norm.size = ft_strlen(curs->main->line);
	norm.del = &curs->main->hdoc;
	norm.str = curs->new;
	norm.i = 0;
	return (norm);
}

int				consult_state(t_term *curs)
{
	int		ret;
	t_norm	norm;

	ret = 0;
	if (curs->prev)
	{
		if (!curs->main)
			curs->main = create_main_line();
	}
	norm = fill_norm(curs);
	ret = parse_incoming_subline(&norm, curs->main->state);
	curs->main->state_before = curs->main->state;
	curs->main->state = ret;
	append_main_line(curs->main, curs->new, curs->main->state);
	if (!(curs->main->state & FAILED))
		ret = determine_next_io_step(curs, ret, &norm);
	else
	{
		return (handle_return_error(-1,
			"syntax error near unexpected token `newline'\n"));
	}
	if (ret == 0)
		ft_putchar_fd('\n', 1);
	return (ret);
}
