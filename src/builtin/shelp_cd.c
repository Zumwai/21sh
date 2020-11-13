/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shelp_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/24 10:08:36 by student           #+#    #+#             */
/*   Updated: 2020/07/30 11:22:57 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int	find_old_pwd_home(char *com, char **tmp)
{
	int		i;

	i = 0;
	while (com[i])
	{
		if (!ft_isletter(com[i]) || com[i] == '\0')
			break ;
		i++;
	}
	if (i == 0)
		return (0);
	(*tmp) = ft_strnew(i + 1);
	(*tmp) = ft_strncpy((*tmp), com, i);
	if (!ft_strnequ((*tmp), PWD, i)
			&& !ft_strnequ((*tmp), OLDPWD, i) && !ft_strnequ((*tmp), HOME, i))
	{
		free((*tmp));
		return (0);
	}
	return (i);
}

static char	*get_path_from_env(char *com, t_env **ev)
{
	t_env	*val;
	char	*tmp;
	char	*path;
	int		index;

	tmp = NULL;
	if (!(index = find_old_pwd_home(com, &tmp)))
		return (NULL);
	val = find_env(ev, tmp);
	if (!val || !val->value)
	{
		free(tmp);
		return (NULL);
	}
	path = ft_strnew(PATH_MAX);
	ft_concat(val->value, &path, com + index);
	free(tmp);
	return (path);
}

static char	*determine_path(char *com, t_env **ev, int i)
{
	char	*path;
	t_env	*cur;

	path = NULL;
	if (com[0] == '/')
		return ((path = ft_strdup(com)));
	if (com[0] == '~')
	{
		if ((cur = find_env(ev, HOME)))
			path = ft_strjoin(cur->value, com + 1);
		return (path);
	}
	if (com[0] == '-')
	{
		if ((cur = find_env(ev, OLDPWD)))
			path = ft_strjoin(cur->value, com + 1);
		return (path);
	}
	if (i)
		path = get_path_from_env(com, ev);
	else
		path = get_full_path(path, com);
	return (path);
}

static void	change_working_dir(char *path, t_env **ev, char *com)
{
	char	**set;

	set = ft_newdim(4);
	set[2] = getcwd(set[2], PATH_MAX);
	if (!chdir(path))
	{
		set[1] = OLDPWD;
		set_env(set, ev);
		set[1] = PWD;
		free(set[2]);
		set[2] = path;
		set_env(set, ev);
		ft_free_tab(set);
	}
	else
	{
		free(set[2]);
		ft_free_tab(set);
		handle_cd_err(check_rights(path, 1), com);
	}
}

int			ft_cd(char **com, t_env **ev)
{
	char	*path;
	int		i;

	i = 0;
	if (com[1])
	{
		if (com[1][0] == '$')
			i = 1;
		if ((path = determine_path(&com[1][i], ev, i)))
			change_working_dir(path, ev, com[1]);
	}
	else
	{
		if ((path = determine_path("~", ev, 0)))
			change_working_dir(path, ev, com[1]);
	}
	if (path)
		free(path);
	else
		handle_empty_error(com[1], ": Variable not set\n");
	return (1);
}
