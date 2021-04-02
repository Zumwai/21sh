/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_word_trie.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 10:47:39 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 12:28:56 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void		insert_suffix(t_trie *curs, char *word, int i, int value)
{
	curs->asc[value] = create_trie_node(word[i]);
	curs->counter++;
	curs = curs->asc[value];
	if (word[i + 1])
	{
		curs->sub = ft_strdup(&word[i]);
	}
	else
	{
		curs->data = word[i];
	}
	curs->leaf = 1;
}

static t_trie	*insert_data_node(t_trie *curs, int value)
{
	curs = curs->asc[value];
	if (curs->sub)
	{
		if (curs->sub[1])
			insert_word_trie(curs, &curs->sub[1]);
		set_free_null(&curs->sub);
		curs->leaf = 0;
	}
	return (curs);
}

t_trie			*insert_word_trie(t_trie *head, char *word)
{
	int		index;
	int		value;
	t_trie	*curs;

	if (!head)
		head = create_trie_node(0);
	curs = head;
	index = 0;
	while (word[index])
	{
		value = convert_asc_value(word[index]);
		if (value < 0)
			handle_exit_errors("Trie value is negative!\n");
		if (!curs->asc[value])
		{
			insert_suffix(curs, word, index, value);
			return (head);
		}
		else
			curs = insert_data_node(curs, value);
		curs->data = word[index];
		index++;
	}
	curs->leaf = 1;
	return (head);
}
