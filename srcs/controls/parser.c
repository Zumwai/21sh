/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 12:58:58 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 14:46:04 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int		cycle_line(t_norm *norm, int state)
{
	int		flag;

	flag = 0;
	while (norm->str[norm->i])
	{
		state = modify_ignored(norm, state, &flag);
		state = handle_heredoc(norm, state);
		if (state & FAILED)
			break ;
		else if (norm->str[norm->i] == '\'' && !(state & D_QUOTE))
			state ^= (QUOTE);
		else if (norm->str[norm->i] == '\"' && !(state & QUOTE))
			state ^= (D_QUOTE);
		else if (!state && norm->str[norm->i] == '<')
			state |= REQ_HDOC;
		else if (!(state & REQ_HDOC) && !(state & ARG_HDOC)
		&& !(state & QUOTE) && !(state & D_QUOTE)
		&& (state & HEREDOC) && norm->str[norm->i] == '<')
			state |= REQ_HDOC;
		norm->i += 1;
	}
	return (state);
}

int		state_endline(t_norm *norm, int state)
{
	norm->i -= 1;
	if (!(state & QUOTE) && norm->str[norm->i] == '\\'
	&& ((norm->i > 0 && norm->str[norm->i - 1] != '\\') || norm->i == 0))
		state |= (GLUE);
	if ((state & ARG_HDOC && !(state & GLUE)))
	{
		printf("failed glue 3\n");
		state |= FAILED;
	}
	if (!(state & GLUE) && (state & READ_HDOC) && norm->str[norm->i + 1] == 0)
	{
		state &= ~(READ_HDOC);
		state &= ~(ARG_HDOC);
		state |= HEREDOC;
		save_coord_hdoc(norm);
	}
	if ((state & REQ_HDOC) && !(state & GLUE))
	{
		printf("failed glue 5\n");
		state |= FAILED;
	}
	return (state);
}

int		parse_incoming_subline(t_norm *norm, int state)
{
	int	i;

	state &= ~(GLUE);
	if (!norm->str[0])
		return (state_save_zero_hdoc(norm, state));
	state = cycle_line(norm, state);
	state = state_endline(norm, state);
	return (state);
}
