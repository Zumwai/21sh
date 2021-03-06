#include "sh.h"

int					check_rights(char *path, int cd)
{
	struct stat		per;
	unsigned int	tmp;
	if ((lstat(path, &per)) == -1)
		return (LSTA);
	tmp = (per.st_mode & S_IFMT);
	if (!cd) {
		if (S_ISDIR(tmp))
			return (NODIR);
		if ((!S_ISREG(tmp)) && !(S_ISLNK(tmp)))
			return (NOEX);
	}
	if (cd)
		if (!S_ISDIR(tmp))
			return (NODIR);
	if (!path)
		return (-42);
	if (access(path, F_OK) == -1)
		return (EXIS);
	if (access(path, X_OK) == -1)
		return (IXUS);
	if (!(S_IXUSR & per.st_mode))
		return (IXUS);
	return (0);
}
/*
static int					is_it_avalible(char *s)
{
	if (access(s, F_OK) == -1)
	{
        handle_empty_error(s, "invalid command\n");
        return (0);
	}
	return (1);
}
*/
/* UNUSED
static void				only_sp(char **line, char target, char change)
{
	int				i;

	i = 0;
	while ((*line)[i] != '\0')
	{
		if ((*line)[i] == target)
			(*line)[i] = change;
		i++;
	}
}

static char				**get_path_variable(t_env **env)
{
	char			**res;
    t_env           *curs;

    if (!(curs = find_env_variable(env, "PATH")))
    {
        //no path variable set
        return NULL;
    }
    return (res = ft_strsplit(curs->value, ':'));
}

static char			*it_path(char *s, t_env **env)
{
	char		**way;
	int			i;
	char		*buf;

	buf = NULL;
	i = 0;
	way = get_path_variable(env);
	if (way == NULL)
		return (s);
	else if (way)
	{
		while (way[i] != NULL)
		{
			if ((buf = ft_strdup(way[i]))) {
				ft_strcat(buf, "/");
				ft_strcat(buf, s);
				if (!check_rights(buf, 0))
					break ;
				set_free_null(&buf);
		//	if (access(buf, F_OK) != -1)
		//		break ;
			}
			i++;

		}
	}
	ft_strsplit_free(&way);
	return (buf);
}

char				*get_path(char *s, t_env **env)
{
	char			*res;
	int				ret;

	ret = 0;
	if (!(res = it_path(s, env)))
		return NULL;
	if (!(ret = check_rights(res, 0)))
	{
		ft_putnbr_fd(ret, 1);
		return (res);
	}
			ft_putnbr_fd(ret, 1);
    set_free_null(&res);
	return (NULL);
}
*/

static char			*is_system_wide(char *com, t_env **ev, int *res)
{
	char	**env_path;
	char	*path;
	t_env	*curs;
	int		i;

	*res = 0;
	path = ft_strnew(PATH_MAX);
	if (!(curs = find_env_variable(ev, "PATH")))
		return (0);
	if (!(env_path = ft_strsplit(curs->value, ':')))
		return (0);
	i = 0;
	while (env_path[i])
	{
		ft_concat(env_path[i], &path, com);
		if (!(*res = check_rights(path, 0)))
			break ;
		i++;
	}
	if (*res <= IXUS)
		handle_return_error(*res, com);
	ft_free_tab(env_path);
	env_path = NULL;
	if ((*res))
	set_free_null(&path);
	return path;
}

static char			*is_local(char *com, t_env **ev, int *res)
{
	char	*path;

	path = NULL;
	if (com[0] != '.' && com[0] != '/')
		return NULL;
	*res = 0;
	if (com[0] == '/')
		path = ft_strdup(com);
	else
		path = get_full_path(path, com);
	if (!(*res = check_rights(path, 0)))
		return path;
	if (*res <= IXUS) {
		handle_return_error(*res, com);
	}
	set_free_null(&path);
	return NULL;
}

char	*get_path(char *com, t_env **env)
{
	char	*path;
	int		search;

	search = 0;
	path = NULL;
	if ((path = is_local(com, env, &search)))
		return path;
	if (search <= IXUS)
		return NULL;
	if ((path = is_system_wide(com, env, &search)))
		return path;
	if (search <= IXUS)
		return NULL;
	handle_empty_error(com, "no such command\n");
	return NULL;
}