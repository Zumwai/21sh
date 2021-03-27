/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto_trie.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 08:47:32 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 11:02:38 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	modify_tilda(char **orig, char **dup, t_env **env)
{
	t_env	*curs;
	char	*path;

	curs = find_env_variable(env, "HOME");
	if (curs && curs->value)
	{
		path = ft_strnew(PATH_MAX);
		ft_strcpy(path, curs->value);
		ft_strcat(path, &(*orig)[1]);
		free(*orig);
		(*orig) = path;
		free(*dup);
		(*dup) = ft_strdup(path);
	}
}

void		parse_trie(char *orig, char *dup, t_yank *buf, t_term *pos)
{
	t_auto	*list;
	char	*new;

	new = NULL;
	list = NULL;
	list = create_new_list(NULL);
	new = search_trie(buf->trie, orig, list);
	if (list->next)
		print_varians(pos, list->next, orig);
	if (new && !new[0])
	{
		if (check_for_dir(dup, new))
			yank_buffer(pos, "/");
	}
	if (new)
		yank_buffer(pos, new);
	set_free_null(&new);
	free(list);
	free_trie_node(buf->trie);
	buf->trie = NULL;
}

int			autocomplete(t_term *pos, t_env **env, t_yank *buf)
{
	char	*orig;
	char	*dup;
	int		len;
	int		com;

	com = 0;
	if (!(orig = get_incomplete(pos, &com)))
		return (1);
	dup = ft_strdup(orig);
	if ('~' == orig[0])
		modify_tilda(&orig, &dup, env);
	len = ft_strlen(orig);
	buf->trie = construct_trie(&orig, env, com);
	if (buf->trie)
		parse_trie(orig, dup, buf, pos);
	else
	{
		if (check_for_dir(orig, ""))
			yank_buffer(pos, "/");
	}
	set_free_null(&dup);
	set_free_null(&orig);
	return (0);
}
