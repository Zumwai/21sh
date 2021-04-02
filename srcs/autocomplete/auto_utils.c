/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 12:09:29 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 12:20:11 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

extern char			*builtin_list(int i)
{
	if (i == 0)
		return ("cd");
	if (i == 1)
		return ("echo");
	if (i == 2)
		return ("clear");
	if (i == 3)
		return ("exit");
	if (i == 4)
		return ("setenv");
	if (i == 5)
		return ("unsetenv");
	if (i == 6)
		return ("set");
	if (i == 7)
		return ("unset");
	if (i == 8)
		return ("export");
	if (i == 9)
		return ("pwd");
	if (i == 10)
		return ("ppid");
	return (NULL);
}

extern void			free_trie_node(t_trie *node)
{
	int	i;

	i = 0;
	if (!node)
		return ;
	while (i < 95)
	{
		if (node->asc[i] != NULL)
		{
			free_trie_node(node->asc[i]);
		}
		i++;
	}
	if (node->sub)
		free(node->sub);
	if (node)
		free(node);
	node = NULL;
}

extern t_trie		*create_trie_node(char c)
{
	t_trie	*new;
	int		i;

	i = 0;
	if (!(new = (t_trie *)malloc(sizeof(t_trie))))
		handle_exit_errors("Malloc returned NULL");
	new->counter = 0;
	new->leaf = 0;
	new->data = c;
	while (i < 95)
	{
		new->asc[i] = NULL;
		i++;
	}
	new->sub = NULL;
	return (new);
}

extern int			convert_asc_value(char c)
{
	if (c >= 32 && c <= 127)
		return (c - 32);
	else
		return (-1);
}

int					is_relative_path(char *orig)
{
	int	i;

	i = 0;
	while (orig[i])
	{
		if (orig[i] == '/')
			return (1);
		i++;
	}
	return (0);
}
