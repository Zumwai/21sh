/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_existence.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 08:56:55 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 10:18:19 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int				get_to_the_diversion(t_trie *node, char **buf, int index)
{
	int i;
	int ret;

	i = 0;
	ret = 0;
	if (!node)
		return (-1);
	if (node->sub)
	{
		index += ft_strlen(node->sub);
		ft_strcat(*buf, node->sub);
		return (0);
	}
	buf[0][index++] = node->data;
	if (node->counter > 1 || node->leaf == true)
		return (-2);
	while (i < 94)
	{
		if (node->asc[i])
			ret = get_to_the_diversion(node->asc[i], buf, index);
		if (ret == -1 || ret == -2)
			return (ret);
		i++;
	}
	return (0);
}

static t_trie	*insert_suffix(t_trie *curs, char *orig, int i, char **rem)
{
	int		size;

	size = ft_strlen(orig);
	if (!ft_strncmp(curs->sub, &orig[i - 1], size - i + 1))
	{
		ft_strcpy(*rem, &curs->sub[size - i + 1]);
		return (curs);
	}
	return (NULL);
}

t_trie			*check_existence(t_trie *head, char *orig, char **remainder)
{
	t_trie	*curs;
	int		value;
	int		i;

	curs = head;
	i = -1;
	while (orig[++i])
	{
		value = convert_asc_value(orig[i]);
		if (!curs->sub && !curs->asc[value])
			return (NULL);
		if (curs->sub)
		{
			return (insert_suffix(curs, orig, i, remainder));
		}
		curs = curs->asc[value];
	}
	if (curs->sub)
		ft_strcpy(*remainder, &curs->sub[1]);
	return (curs);
}
