/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 12:42:54 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 14:15:18 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"


static void append_main_line(t_actual *main, char *sub, int state)
{
    char    *new;
    int     size;

	size = 0;
	new = NULL;
	if (sub)
   		size += ft_strlen(sub) + 1;
	if (main->line)
		size += ft_strlen(main->line);
	if (size == 1)
		size++;
	if (!(state & GLUE) && ((state & HEREDOC) || (state & QUOTE) || (state & D_QUOTE)))
		size++;
    if (!(new = ft_strnew(size + 1)))
		handle_exit_errors("Malloc returned NULL");
    if (main->line)
        ft_strcat(new, main->line);
    ft_strcat(new, sub);
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
	if (main->line)
	{
		free(main->line);
		main->line = NULL;
	}
	main->size = ft_strlen(new);
	main->line = new;
	main->line[size] = 0;
}

static t_term *create_next_io(t_actual **line, int y)
{
	t_term *input;

	input = create_new_io_struct(line);
	input->y = y;
	if (*line)
		input->main = *line;
	return input;
}

int		check_hdoc_eot(t_hdoc **head, char *sub)
{
	t_hdoc	*curs;

	curs = *head;
	if (!curs)
		return (1);
	while (curs)
	{
		if (curs->used == false)
		{
			if (ft_strequ(sub, curs->eot))
			{
				curs->used = true;
				if (!curs->next)
					return 0;
				return (1);
			}
			break ;
		}
		curs = curs->next;
	}
	return (1);
}

int		determine_next_io_step(t_term *curs, int ret, t_norm *norm)
{
	int		res;

	res = 1;
	if (curs->main->state & (FAILED))
		return (-1); /* handle errors */
	if (!curs->main->state)
		return (0); /* check for ending */
	if ((curs->main->state & HEREDOC))
	{
		res = update_hdoc_list(&curs->main->hdoc, curs->main->line);
		if (!res)
		{
			res = check_hdoc_eot(&curs->main->hdoc, curs->new);
		}
		if (res)
		{
			curs->next = create_next_io(&curs->main, curs->y);
			curs->next->prev = curs;
		}
	}
	else
	{
		curs->next = create_next_io(&curs->main, curs->y);
		curs->next->prev = curs;
	}
	return (res);
}

int		consult_state(t_term *curs)
{
	int		ret;
	t_norm	norm;

	ret = 0;
	if (curs->prev) /* just to be safe */
		if (!curs->main)
			curs->main = create_main_line();
	norm.size = ft_strlen(curs->main->line);
	norm.del = &curs->main->hdoc;
	norm.str = curs->new;
	norm.i = 0;
	ret = parse_incoming_subline(&norm, curs->main->state);
	curs->main->state_before = curs->main->state;
	curs->main->state = ret;
	append_main_line(curs->main, curs->new, curs->main->state);
	if (!(curs->main->state & FAILED))
		ret = determine_next_io_step(curs, ret, &norm);
	else {
		return (handle_return_error(-1, "syntax error near unexpected token `newline'\n"));
	}
	if (ret == 0)
		ft_putchar_fd('\n', 1);
	return (ret);
}