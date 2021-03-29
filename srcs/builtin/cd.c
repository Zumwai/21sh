#include "sh.h"

static void		change_working_dir(char *path, t_env **env, char *com, int flag)
{
	char		*pwd;
	t_env		*cur;

	pwd = NULL;
	pwd = getcwd(pwd, PATH_MAX);
	if (!chdir(path))
	{
		sh_setnew("OLDPWD", pwd, env, 1);
		if (flag == PHYSICAL)
		{
			set_free_null(&pwd);
			pwd = getcwd(pwd, PATH_MAX);
			sh_setnew("PWD", pwd, env, 1);
		}
		else
			sh_setnew("PWD", path, env, 1);
	}
	else
		handle_cd_err(check_rights(path, 1), com);
	free(pwd);
}

char		*get_value_env(char *sought, t_env **env)
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

static char	*create_first_part(char *com, t_env **env, int flag, int *size)
{
	char *path;
	char *pwd;

	pwd = NULL;
	path = ft_strnew(PATH_MAX);
	if (com[0] != '/' && com[0] != '~')
	{
		if (flag == PHYSICAL)
			pwd = getcwd(pwd, PATH_MAX);	
		else
			pwd = get_value_env("PWD", env);
		if (!pwd)
			pwd = getcwd(pwd, PATH_MAX);
		ft_strcat(path, pwd);
	}
	if (com[0] == '~')
	{
		pwd = get_value_env("HOME", env);
		if (pwd)
			ft_strcat(path, pwd);
	}
	if (com[0] == '/')
		path[0] = '/';
	*size = PATH_MAX;
	if (pwd)
		free(pwd);
	return (path);
}

char	*clear_dotdot(char *path, int flag)
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

char	*concatenate_smart(char *path, char *sep, int *size)
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

char		*trim_curpath(char *curpath, t_env **env) 
{
	char	*pwd;
	int		i;
	int		size;

	i = 0;
	pwd = NULL;
	size = ft_strlen(curpath);
	if (size < 4096)
		return (curpath);
	if (!(pwd =  get_value_env("PWD", env)))
	{
		if (!(pwd = getcwd(pwd, PATH_MAX)))
			return (NULL);
	}
	while (pwd[i] == curpath[i])
		i++;
	ft_memmove(curpath, &curpath[i], size - i);
	curpath[size - i] = 0;
	size = ft_strlen(curpath);
	if (size >= 4096)
		set_free_null(&curpath);
	return (curpath);
}

char	*create_path(char *com, t_env **env, int flag)
{
	char	**sep;
	char	*path;
	int		i;
	int		size;

	i = -1;
	path = create_first_part(com, env, flag, &size);
	if (com[0] == '~')
		i++;
	sep = ft_strsplit(com, '/');
	while (sep[++i])
	{
		if ((flag == LOGICAL || flag == DEFAULT) && ft_strequ(sep[i], ".."))
			clear_dotdot(path, flag);
		else if (!ft_strequ(sep[i], "."))
			path = concatenate_smart(path, sep[i], &size);
		check_rights(path, 1);
	}
	if (sep)
		ft_strsplit_free(&sep);
	path = trim_curpath(path, env);
	ft_free_tab(sep);
	sep = NULL;
	return (path);
}

char	*get_stdpath(char **com, t_env **env)
{
	char *curpath;

	curpath = NULL;
	if (!com[1])
		curpath = get_value_env("HOME", env);
	else if (ft_strequ(com[1], "-"))
	{
		curpath = get_value_env("OLDPWD", env);
		if (curpath)
			ft_putendl(curpath);
	}
	return (curpath);
}

int		sh_cd(char **com, t_env **env)
{
	char		*curpath;
	int			i;
	int			flag;
	size_t		size;

	curpath = NULL;
	flag = DEFAULT;
	i = 1;
	if (!com[1] || ft_strequ(com[1], "-"))
		curpath = get_stdpath(com, env);
	else if (ft_strequ(com[1], "-P"))
		flag = PHYSICAL;
	else if (ft_strequ(com[1], "-L"))
		flag = LOGICAL;
	if (flag != DEFAULT)
		i++;
	if (!curpath)
		curpath = create_path(com[i], env, flag);
	if (curpath)
		change_working_dir(curpath, env, com[i], flag);
	free(curpath);
	return (1);
}
