/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 12:58:58 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 14:23:37 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static char	find_next_char(char *str, int i)
{
	if (i < 0)
		return (0);
	while (str[i])
	{
		if (str[i] != ' ')
			return (str[i]);
		i++;
	}
	return (0);
}

static int	check_for_zero(char *str, int i)
{
	if (i < 0)
		return (0);
	while (str[i])
	{
		if (str[i] == '\\')
		{
			if (str[i + 1] == 0)
				return (1);
			else
				return (0);
		}
		break ;
		i++;
	}
	if (str[i] == 0)
		return (1);
	return (0);
}

int		verify_char_heredoc(char c)
{
	if (c == '|' || c == '&' || c == ';'
	|| c == '\n' || c == 0 || c == '<' || c == '>')
		return (1);
	return (0);
}

static int		verify_end_arg(char c, int n, int state)
{
	if (c == '\\' && !n)
		return (0);
	if (c != '\\' && !n)
		return (1);
	if (c == '\'' || c == '\"')
		return (0);
	if (verify_char_heredoc(c))
	{
		if ((state & GLUE))
			return (0);
		return (1);
	}
	if (c == ' ')
		return (1);
	return (0);
}

static int		state_save_zero_hdoc(t_norm *norm, int state)
{
	if ((state & READ_HDOC))
	{
		state ^= READ_HDOC;
		save_coord_hdoc(norm);
	}
	return (state);
}

int		unignore_state(t_norm *norm, int *flag, int state)
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

int		skip_ignored(t_norm *norm, int *flag, int state)
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

int		begin_read_hdoc(t_norm *norm, int state)
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

int		stop_read_hdoc(t_norm *norm, int state)
{
	if ((verify_end_arg(norm->str[norm->i], norm->str[norm->i + 1], state)
	|| (norm->i == 0 && norm->str[norm->i] != '\\' && !norm->str[norm->i + 1])))
	{
		state ^= READ_HDOC;
		save_coord_hdoc(norm);
	}
	return (state);
}

int		check_valid_hdoc(t_norm *norm, int state)
{
	char c;

	if (norm->str[norm->i] == '<')
	{ /* dont need to check it here, useless and harmful */
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
	}
	else if (norm->i > 0 && norm->str[norm->i] == '<' && norm->str[norm->i - 1] == '<'
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

int		cycle_line(t_norm *norm, int state)
{
	int		flag;

	flag = 0;
	while (norm->str[norm->i])
	{
		if (flag != IGNORE)
			state = unignore_state(norm, &flag, state);
		else if (flag == IGNORE)
			state = skip_ignored(norm, &flag, state);
		if ((state & ARG_HDOC))
			state = begin_read_hdoc(norm, state);
		else if ((state & READ_HDOC))
			state = stop_read_hdoc(norm, state);
		else if ((state & REQ_HDOC))
		{
			state = check_valid_hdoc(norm, state);
			if (state & FAILED)
				break ;
		}
		else if (norm->str[norm->i] == '\'' && !(state & D_QUOTE))
			state ^= (QUOTE);
		else if (norm->str[norm->i] == '\"'	&& !(state & QUOTE))
			state ^= (D_QUOTE);
		else if (!state && norm->str[norm->i] == '<')
			state |= REQ_HDOC;
		else if (!(state & REQ_HDOC) && !(state & ARG_HDOC) &&!(state & QUOTE) && !(state & D_QUOTE) && (state & HEREDOC) && norm->str[norm->i] == '<')
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
	if ((state & ARG_HDOC && !(state & GLUE))) {
		printf("failed glue 3\n");
		state |= FAILED;
	}
	if (!(state & GLUE) && (state & READ_HDOC) && norm->str[norm->i  + 1] == 0) {
		state &= ~ (READ_HDOC);
		state &= ~ (ARG_HDOC);
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
