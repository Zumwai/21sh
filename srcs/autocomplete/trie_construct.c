/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trie_construct.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 11:11:02 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 12:36:34 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static t_trie		*init_auto_trie(char *original, t_env **env)
{
	t_trie	*head;
	t_env	*way;
	int		len;
	char	**ways;
	char	*pwd;

	pwd = NULL;
	head = NULL;
	way = find_env_variable(env, "PATH");
	ways = ft_strsplit(way->value, ':');
	if (!(pwd = getcwd(pwd, PATH_MAX)))
		return (NULL);
	head = fill_variant_list(original, pwd, head);
	head = fill_env_path(head, original, env, ways);
	free(pwd);
	ft_strsplit_free(&ways);
	return (head);
}

t_trie				*construct_env_trie(char *orig, t_env **env)
{
	t_env	*curs;
	t_trie	*head;

	head = create_trie_node(0);
	head->asc[4] = create_trie_node('$');
	curs = *env;
	while (curs)
	{
		head->asc[4] = insert_word_trie(head->asc[4], curs->name);
		curs = curs->next;
	}
	return (head);
}

t_trie				*construct_local_entry(char *original, int flag)
{
	t_trie	*head;
	char	*pwd;
	char	*path;
	int		len;

	pwd = NULL;
	head = NULL;
	len = ft_strlen(original);
	if (!(pwd = getcwd(pwd, PATH_MAX)))
		return (NULL);
	path = ft_strjoin(pwd, original);
	if (flag == DEFAULT)
		head = fill_variant_list(original, pwd, head);
	if (flag == DIRECTORY || flag == LOC_DIRECTORY)
	{
		ft_strclr(original);
		head = fill_variant_dirs(original, pwd, head);
	}
	if (flag == LOC_FINISH)
		head = fill_variant_dirs(original, pwd, head);
	free(pwd);
	free(path);
	return (head);
}

t_trie				*construct_trie(char **orig, t_env **env, int source)
{
	t_trie	*head;
	int		len;

	head = NULL;
	len = ft_strlen(*orig);
	if ((*orig)[0] == '$')
		head = construct_env_trie(*orig, env);
	else if ((*orig)[0] == '.' && (*orig)[1] == '/')
	{
		ft_memmove(*orig, &(*orig)[2], ft_strlen(*orig) - 1);
		head = construct_local_entry(*orig, DEFAULT);
	}
	else if (!source)
		head = init_auto_trie(*orig, env);
	else if (len > 0 && (*orig)[len - 1] == '/')
		head = fill_local_dir(orig, len, source);
	else if (is_relative_path(*orig))
		head = fill_relative_path(orig);
	else
		head = construct_local_entry(*orig, DEFAULT);
	if (!head)
		return (NULL);
	if (head)
		head->data = 0;
	return (head);
}
