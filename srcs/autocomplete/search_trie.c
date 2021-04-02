/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_trie.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 09:29:34 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 12:21:55 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static char	*insert_upto(t_trie *curs, char *comp, char *orig, t_auto *list)
{
	int		index;
	int		res;
	char	*ret;

	ret = NULL;
	ft_strcpy(comp, orig);
	index = ft_strlen(comp);
	if (index > 0)
		index--;
	res = get_to_the_diversion(curs, &comp, index++);
	if (ft_strcmp(comp, orig))
		ret = ft_strdup(&comp[index]);
	if (res < 0)
	{
		if (res == -1)
			ft_strcpy(comp, orig);
		print_words(curs, &comp, 0, list);
	}
	free(comp);
	return (ret);
}

char		*search_trie(t_trie *head, char *orig, t_auto *list)
{
	t_trie	*curs;
	char	*comp;
	char	*ret;
	int		index;
	int		res;

	ret = NULL;
	comp = ft_strnew(NAME_MAX + 1);
	curs = check_existence(head, orig, &comp);
	if (!curs)
		free(comp);
	else if (!curs->sub)
	{
		return (insert_upto(curs, comp, orig, list));
	}
	else if (curs->sub)
		return (comp);
	return (NULL);
}
