/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 14:42:28 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 14:43:06 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

char	find_next_char(char *str, int i)
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

int	check_for_zero(char *str, int i)
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

int		verify_end_arg(char c, int n, int state)
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
