#include "sh.h"

static void	change_working_dir(char *path, t_env **env, char *com, int flag)
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

char	*get_value_env(char *sought, t_env **env)
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

char	*create_path(char *com, t_env **env, int flag)
{
	char	**sep = NULL;
	char	*curpath = NULL;
	char	*path;
	char	*pwd = NULL;
	t_env	*curs = NULL;
	int		i = 0;

	path = ft_strnew(PATH_MAX);
	if (com[0] != '/' && com[0] != '~') {
		if (flag == PHYSICAL)
			pwd = getcwd(pwd, PATH_MAX);
		else
			pwd = get_value_env("PWD", env);
		if (!pwd)
			pwd = getcwd(pwd, PATH_MAX);
		ft_strcat(path, pwd);
	}
	sep = ft_strsplit(com, '/');
	if (com[0] == '~')
	{
		pwd = get_value_env("HOME", env);
		if (pwd)
			ft_strcat(path, pwd);
		i++;
	}
	if (com[0] == '/')
		path[0] = '/';
	int		j = 0;
	while (sep[i])
	{
		j = 0;
		if ((flag == LOGICAL || flag == DEFAULT) && ft_strequ(sep[i], ".."))
		{
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
		}
		else {
			ft_strcat(path, "/");
			ft_strcat(path, sep[i]);
		}
		check_rights(path, 1);
		i++;
	}
	free(pwd);
	curpath = ft_strdup(path);
	ft_free_tab(sep);
	free(path);
	sep = NULL;
	return (curpath);
}

int	trim_curpath(char **curpath, t_env **env) 
{
	char	*pwd;
	int		i;
	int		size;

	i = 0;
	pwd = NULL;
	size = ft_strlen(*curpath);
	if (!(pwd =  get_value_env("PWD", env)))
	{
		if (!(pwd = getcwd(pwd, PATH_MAX)))
			return (1);
	}
	while (pwd[i] == *curpath[i])
		i++;
	ft_memmove(*curpath, &(*curpath)[i], size - i);
	(*curpath)[size - i] = 0;
	if (pwd)
		free(pwd);
	return (size - i);
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
	if (!com[1])
		curpath = get_value_env("HOME", env);
	else if (ft_strequ(com[1], "-"))
	{
		curpath = get_value_env("OLDPWD", env);
		if (curpath)
			ft_putendl(curpath);
	}
	else if (ft_strequ(com[1], "-P"))
		flag = PHYSICAL;
	else if (ft_strequ(com[1], "-L"))
		flag = LOGICAL;
	if (flag != DEFAULT)
		i++;
	if (!curpath)
		curpath = create_path(com[i], env, flag);
	if (curpath)
	{
		size = ft_strlen(curpath);
		if (size >= 4096)
			size = trim_curpath(&curpath, env);
		if (size <= 4096)
			change_working_dir(curpath, env, com[i], flag);
	}
	free(curpath);
	return (1);
}
