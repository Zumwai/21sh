/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto_state.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 07:53:56 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 10:18:22 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int		is_delimeter(char c)
{
	if (c == ';' || c == '&' || c == '\"' || c == '\''
	|| c == '\t' || c == '\v' || c == '\n' || c == '|' || c == '&')
		return (1);
	return (0);
}

static int		get_pos(t_term *pos, int curr)
{
	int	ret;

	ret = 0;
	while (curr > 0)
	{
		ret = 0;
		if (pos->new[curr] == ' '
		|| pos->new[curr] == '\t' || pos->new[curr] == '\v')
			curr--;
		else if (is_delimeter(pos->new[curr]))
		{
			ret = 0;
			break ;
		}
		else if (ft_isascii(pos->new[curr]))
		{
			ret = 1;
			break ;
		}
		else
			curr--;
	}
	return (ret);
}

char			*get_incomplete(t_term *pos, int *cd)
{
	int		curr;
	int		end;
	char	*incomplete;
	char	*test;

	test = NULL;
	incomplete = NULL;
	curr = pos->index + pos->delta_x;
	if (pos->index == 0 || curr <= 0)
		return (NULL);
	end = curr;
	while (curr > 0 && ft_ischar(pos->new[curr - 1])
	&& pos->new[curr - 1] != ';' && pos->new[curr - 1] != '|'
	&& pos->new[curr] != '&' && pos->new[curr] != '$')
		curr--;
	incomplete = ft_strsub(pos->new, curr, end - curr);
	curr--;
	*cd = get_pos(pos, curr - 1);
	return (incomplete);
}
