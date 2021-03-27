/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 10:41:58 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 10:43:25 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static t_auto	*find_last(t_auto *head)
{
	t_auto *curs;

	curs = head;
	if (!curs)
		return (NULL);
	while (curs->next)
		curs = curs->next;
	return (curs);
}

static void		insert_data(char **line, char *sub, int *index, char data)
{
	if (sub)
	{
		ft_strcat(*line, sub);
		(*index) += ft_strlen(sub);
	}
	else if (data)
	{
		line[0][*index] = data;
		line[0][*index + 1] = '\0';
		(*index)++;
	}
}

void			print_words(t_trie *node, char **line, int index, t_auto *list)
{
	t_auto	*curs;
	int		i;

	i = 0;
	if (!node)
		return ;
	if (line[0][index])
		line[0][index] = 0;
	if (node->data != -1)
		insert_data(line, node->sub, &index, node->data);
	if (node->leaf == true)
	{
		curs = find_last(list);
		curs->next = create_new_list(*line);
	}
	while (i < 94)
	{
		if (node->asc[i])
			print_words(node->asc[i], line, index, list);
		i++;
	}
}
