/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filler.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 12:06:06 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 12:19:41 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

extern t_trie		*fill_variant_list(char *orig, char *path, t_trie *head)
{
	struct dirent	*container;
	DIR				*dir;
	int				len;

	len = 0;
	dir = opendir(path);
	if (!dir)
		return (head);
	if (!orig)
		return (NULL);
	len = ft_strlen(orig);
	while ((container = readdir(dir)))
	{
		if (container->d_reclen == 0)
			break ;
		if ((orig[0] == 0 || ft_strnequ(container->d_name, orig, len))
		&& ft_strcmp(container->d_name, ".")
		&& ft_strcmp(container->d_name, ".."))
		{
			head = insert_word_trie(head, container->d_name);
		}
	}
	free(dir);
	return (head);
}

extern t_trie		*fill_variant_dirs(char *orig, char *path, t_trie *head)
{
	struct dirent	*container;
	DIR				*dir;
	int				len;

	dir = opendir(path);
	if (!dir)
		return (head);
	if (!orig)
		return (NULL);
	len = ft_strlen(orig);
	while ((container = readdir(dir)))
	{
		if (container->d_reclen == 0)
			break ;
		if ((orig[0] == 0 || (ft_strnequ(container->d_name, orig, len))))
			insert_var_dir(path, container->d_name, head);
	}
	free(dir);
	return (head);
}

extern t_trie		*fill_env_path(t_trie *head, char *orig,
					t_env **env, char **ways)
{
	char	*bilt;
	int		i;
	int		len;

	len = ft_strlen(orig);
	i = -1;
	while (++i <= 10)
	{
		bilt = builtin_list(i);
		if (ft_strnequ(bilt, orig, len))
			head = insert_word_trie(head, builtin_list(i));
	}
	i = -1;
	while (ways[++i])
		head = fill_variant_list(orig, ways[i], head);
	return (head);
}

extern t_trie		*fill_relative_path(char **orig)
{
	t_trie	*head;
	char	*path;
	char	*sub;

	path = NULL;
	head = NULL;
	sub = split_path(*orig, &path);
	head = fill_variant_list(sub, path, head);
	if (*orig)
		free(*orig);
	*orig = sub;
	if (path)
		free(path);
	return (head);
}

extern t_trie		*fill_local_dir(char **orig, int len, int source)
{
	t_trie *head;

	head = NULL;
	len = ft_strlen(*orig);
	if (orig[0][len - 1] == '/')
		head = fill_variant_list("", *orig, head);
	else
		head = construct_local_entry(*orig, source);
	*orig[0] = 0;
	return (head);
}
