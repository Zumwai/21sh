/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 14:45:20 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 14:47:43 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int		check_validity(t_norm *norm, int state)
{
	char	c;

	c = find_next_char(norm->str, norm->i + 1);
	if (verify_char_heredoc(c))
	{
		printf("failed verification 2%c\n", c);
		state |= FAILED;
	}
	else if (c == '\\' && check_for_zero(norm->str, norm->i + 1))
	{
		state ^= ARG_HDOC;
		state ^= REQ_HDOC;
	}
	else
	{
		state ^= ARG_HDOC;
		state ^= REQ_HDOC;
	}
	if (norm->str[1] == 0)
	{
		printf("failed sudden end of the line\n");
		state |= FAILED;
	}
	return (state);
}

static int		check_valid_hdoc(t_norm *norm, int state)
{
	if (norm->str[norm->i] == '<')
	{
		state = check_validity(norm, state);
	}
	else if (norm->i > 0 && norm->str[norm->i] == '<'
	&& norm->str[norm->i - 1] == '<'
	&& !(state & QUOTE) && !(state & D_QUOTE))
	{
		state ^= ARG_HDOC;
		state ^= REQ_HDOC;
	}
	else if (norm->str[norm->i] != '\\')
	{
		state &= ~(REQ_HDOC);
	}
	return (state);
}

static int		begin_read_hdoc(t_norm *norm, int state)
{
	char c;

	c = find_next_char(norm->str, norm->i);
	if (verify_char_heredoc(c))
	{
		printf("failed verification 1%c\n", c);
		state |= FAILED;
	}
	else if (norm->str[norm->i] != ' ' && c != '\\'
	&& !check_for_zero(norm->str, norm->i))
	{
		state ^= ARG_HDOC;
		state ^= READ_HDOC;
		state |= HEREDOC;
	}
	return (state);
}

static int		stop_read_hdoc(t_norm *norm, int state)
{
	if ((verify_end_arg(norm->str[norm->i], norm->str[norm->i + 1], state)
	|| (norm->i == 0 && norm->str[norm->i] != '\\' && !norm->str[norm->i + 1])))
	{
		state ^= READ_HDOC;
		save_coord_hdoc(norm);
	}
	return (state);
}

int		handle_heredoc(t_norm *norm, int state)
{
	if ((state & ARG_HDOC))
		state = begin_read_hdoc(norm, state);
	else if ((state & READ_HDOC))
		state = stop_read_hdoc(norm, state);
	else if ((state & REQ_HDOC))
	{
		state = check_valid_hdoc(norm, state);
	}
	return (state);
}