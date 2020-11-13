/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/24 10:10:28 by student           #+#    #+#             */
/*   Updated: 2020/07/30 11:34:37 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int					check_rights(char *path, int cd)
{
	struct stat		per;
	unsigned int	tmp;

	if ((lstat(path, &per)) == -1)
		return (LSTA);
	if (access(path, F_OK) == -1)
		return (EXIS);
	if (access(path, X_OK) == -1)
		return (IXUS);
	if (!(S_IXUSR & per.st_mode))
		return (IXUS);
	tmp = (per.st_mode & S_IFMT);
	if (!cd)
		if ((!S_ISREG(tmp)) && !(S_ISLNK(tmp)))
			return (NOEX);
	if (cd)
		if (!S_ISDIR(tmp))
			return (NODIR);
	return (0);
}

static void			*is_builtin(char **com)
{
	if (ft_strequ(com[0], "cd"))
		return (&ft_cd);
	if (ft_strequ(com[0], "pwd"))
		return (&ft_pwd);
	if (ft_strequ(com[0], "echo"))
		return (&ft_echo);
	if (ft_strequ(com[0], "setenv"))
		return (&set_env);
	if (ft_strequ(com[0], "unsetenv"))
		return (&unset_env);
	if (ft_strequ(com[0], "printenv"))
		return (&display_env_list);
	if (ft_strequ(com[0], "exit"))
		return (&ft_exit);
	if (ft_strequ(com[0], "ppid"))
		return (&display_id_kid_parent);
	if (ft_strequ(com[0], "env"))
		return (&manage_env);
	if (ft_strequ(com[0], "help"))
		return (&ft_help);
	return (NULL);
}

static int			is_system_wide(char **com, t_env **ev)
{
	char	**env_path;
	char	*path;
	t_env	*index;
	int		i;

	path = ft_strnew(PATH_MAX);
	if (!(index = find_env(ev, "PATH")))
		return (0);
	if (!(env_path = ft_strsplit(index->value, ':')))
		return (0);
	i = 0;
	while (env_path[i])
	{
		ft_concat(env_path[i], &path, com[0]);
		if (!check_rights(path, 0))
		{
			ft_free_tab(env_path);
			return (create_child_cmd(ev, com, path));
		}
		i++;
	}
	ft_free_tab(env_path);
	free(path);
	return (0);
}

static int			is_local(char **com, t_env **ev)
{
	char	*path;
	int		res;

	path = NULL;
	if (com[0][0] != '.' && com[0][0] != '/')
		return (0);
	res = 0;
	if (com[0][0] == '/')
		path = ft_strdup(com[0]);
	else
		path = get_full_path(path, com[0]);
	if (!(res = check_rights(path, 0)))
		return (create_child_cmd(ev, com, path));
	if (res <= IXUS)
		handle_return_error(res, com[0]);
	free(path);
	return (res);
}

int					go_do(char **com, t_env **ev)
{
	int		(*f)(char **, t_env **);
	int		res;

	if ((f = is_builtin(com)))
		return (f(com, ev));
	res = is_local(com, ev);
	if (res <= IXUS || res == 1)
		return (res);
	res = is_system_wide(com, ev);
	if (res <= IXUS || res == 1)
		return (res);
	ft_putstr_fd(com[0], 2);
	ft_putstr_fd(": commad not found\n", 2);
	return (1);
}
