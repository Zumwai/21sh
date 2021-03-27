/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_states.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 14:43:59 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 17:49:47 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int		state_save_zero_hdoc(t_norm *norm, int state)
{
	if ((state & READ_HDOC))
	{
		state ^= READ_HDOC;
		save_coord_hdoc(norm);
	}
	return (state);
}

static int		unignore_state(t_norm *norm, int *flag, int state)
{
	if (norm->str[norm->i] == '\\')
	{
		if (norm->str[norm->i + 1] != 0)
		{
			*flag = IGNORE;
			if (state & REQ_HDOC)
				state &= ~(REQ_HDOC);
		}
	}
	return (state);
}

static int		skip_ignored(t_norm *norm, int *flag, int state)
{
	*flag = DEFAULT;
	if (state & ARG_HDOC)
	{
		state &= ~(ARG_HDOC);
		state ^= READ_HDOC;
		state |= HEREDOC;
	}
	norm->i += 1;
	return (state);
}

int		modify_ignored(t_norm *norm, int state, int *flag)
{
	if (*flag != IGNORE)
		state = unignore_state(norm, flag, state);
	else if (*flag == IGNORE)
		state = skip_ignored(norm, flag, state);
	return (state);
}
