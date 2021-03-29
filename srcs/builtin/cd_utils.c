/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 11:38:40 by aophion           #+#    #+#             */
/*   Updated: 2021/03/29 12:07:40 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

char			*get_value_env(char *sought, t_env **env)
{
	t_env	*curs;
	char	*new;

	new = NULL;
	curs = NULL;
	curs = find_env_variable(env, sought);
	if (curs && curs->value)
		new = ft_strdup(curs->value);
	return (new);
}

extern char		*concatenate_smart(char *path, char *sep, int *size)
{
	int		len_p;
	int		len_s;
	char	*new;

	len_p = ft_strlen(path);
	len_s = ft_strlen(sep);
	if (len_p + len_s >= *size)
	{
		new = ft_strnew(len_p + len_s + 1);
		ft_strcpy(new, path);
		free(path);
		path = new;
		*size = len_p + len_s + 1;
	}
	ft_strcat(path, "/");
	ft_strcat(path, sep);
	return (path);
}

extern char		*clear_dotdot(char *path, int flag)
{
	int	j;

	j = ft_strlen(path);
	while (path[j] != '/' && j > 0)
	{
		path[j] = 0;
		j--;
	}
	while (j > 0 && path[j] == '/')
	{
		path[j] = 0;
		j--;
	}
	return (path);
}

static char		*insert_pwd(char *path, t_env **env, int flag)
{
	char *pwd;

	pwd = NULL;
	if (flag == PHYSICAL)
		pwd = getcwd(pwd, PATH_MAX);
	else
		pwd = get_value_env("PWD", env);
	if (!pwd)
		pwd = getcwd(pwd, PATH_MAX);
	if (pwd)
	{
		if (ft_strlen(pwd) < PATH_MAX)
			ft_strcat(path, pwd);
		free(pwd);
	}
	return (path);
}

char			*create_first_part(char *com, t_env **env, int flag, int *size)
{
	char	*path;
	char	*pwd;

	pwd = NULL;
	path = ft_strnew(PATH_MAX);
	if (com[0] != '/' && com[0] != '~')
		insert_pwd(path, env, flag);
	else if (com[0] == '~')
	{
		pwd = get_value_env("HOME", env);
		if (pwd && ft_strlen(pwd) < PATH_MAX)
			ft_strcat(path, pwd);
	}
	if (com[0] == '/')
		path[0] = '/';
	*size = PATH_MAX;
	if (pwd)
		free(pwd);
	return (path);
}
