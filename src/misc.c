/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/29 15:39:24 by student           #+#    #+#             */
/*   Updated: 2020/07/29 22:43:34 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_concat(char *str, char **path, char *name)
{
	ft_strclr((*path));
	(*path) = ft_strcpy((*path), str);
	(*path) = ft_strcat((*path), "/");
	if (name)
		(*path) = ft_strcat((*path), name);
}

char	*get_full_path(char *path, char *name)
{
	char	*str;
	char	buf[PATH_MAX];

	if (!(str = getcwd(buf, PATH_MAX)))
		return (NULL);
	path = ft_strnew(PATH_MAX);
	ft_strclr(path);
	path = ft_strcpy(path, str);
	path = ft_strcat(path, "/");
	path = ft_strcat(path, name);
	return (path);
}

void	ft_free_tab(char **tab)
{
	int		i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
}
