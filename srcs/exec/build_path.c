#include "sh.h"

/* For future use
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
*/
static int					is_it_avalible(char *s)
{
	if (access(s, F_OK) == -1)
	{
        handle_empty_error(s, "invalid command");
        return (0);
	}
	return (1);
}

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
*/
static char				**get_ways(t_env **env)
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
	way = get_ways(env);
	if (way == NULL)
		return (s);
	else if (way)
	{
		while (way[i] != NULL)
		{
			buf = ft_strdup(way[i]);
			ft_strcat(buf, "/");
			ft_strcat(buf, s);
			if (access(buf, F_OK) != -1)
				break ;
			i++;
            set_free_null(&buf);
		}
	}
	ft_strsplit_free(&way);
	return (buf);
}

char				*get_path(char *s, t_env **env)
{
	char			*res;

	res = it_path(s, env);
	if (is_it_avalible(res))
	{
		return (res);
	}
    set_free_null(&res);
	return (NULL);
}