#include "sh.h"

void		handle_cd_err(int num, char *name)
{
	ft_putstr_fd("-shelp!: ", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	if (num == -4)
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	else if (num == -2 || num == -3)
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	else if (num == -7)
		ft_putstr_fd(": Not a directory\n", STDERR_FILENO);
}
/*
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
			&& !ft_strnequ((*tmp), "OLDPWD", i) && !ft_strnequ((*tmp), "HOME", i))
	{
		free((*tmp));
		return (0);
	}
	return (i);
}

static char	*get_path_from_env(char *com, t_env **ev)
{
	t_env	*value;
	char	*tmp;
	char	*path;
	int		index;

	tmp = NULL;
	if (!(index = find_old_pwd_home(com, &tmp)))
		return (NULL);
	value = find_env_variable(ev, tmp);
	if (!value || !value->value)
	{
		set_free_null(&tmp);
		return (NULL);
	}
	path = ft_strnew(PATH_MAX);
	ft_concat(value->value, &path, com + index);
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
		if ((cur = find_env_variable(ev, "HOME")))
			path = ft_strjoin(cur->value, com + 1);
		return (path);
	}
	if (com[0] == '-')
	{
		if ((cur = find_env_variable(ev, "OLDPWD")))
			path = ft_strjoin(cur->value, com + 1);
		return (path);
	}
	if (i)
		path = get_path_from_env(com, ev);
	else
		path = get_full_path(path, com);
	return (path);
}

static void	change_working_dir(char *path, t_env **env, char *com)
{
	
	char	**set;

	set = ft_newdim(4);
	set[2] = getcwd(set[2], PATH_MAX);
	if (!chdir(path))
	{
		set[1] = "OLDPWD";
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
		ft_free_tab(&set);
		handle_cd_err(check_rights(path, 1), com);
	}
	

	char		*pwd;
	//char		*old_pwd;
	t_env		*cur;

	pwd = NULL;
	pwd = getcwd(pwd, PATH_MAX);
	if (!chdir(path))
	{
		sh_setnew("OLDPWD", pwd, env);
		sh_setnew("PWD", path, env);
	}
	else
		{
		//handle_cd_err
		handle_cd_err(check_rights(path, 1), com);
	}
	free(pwd);
	//set_free_null(&pwd);
	//set_free_null(&old_pwd);
}

int			sh_cd(char **com, t_env **ev)
{
	char	*path;
	int		i;

	path = NULL;
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
*/

static void	change_working_dir(char *path, t_env **env, char *com, int flag)
{
	char		*pwd;
	//char		*old_pwd;
	t_env		*cur;

	pwd = NULL;
	pwd = getcwd(pwd, PATH_MAX);
	if (!chdir(path))
	{
		sh_setnew("OLDPWD", pwd, env);
		if (flag == PHYSICAL)
		{
			set_free_null(&pwd);
			pwd = getcwd(pwd, PATH_MAX);
			sh_setnew("PWD", pwd, env);
		}
		else
			sh_setnew("PWD", path, env);
	}
	else
		{
		//handle_cd_err
		handle_cd_err(check_rights(path, 1), com);
	}
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
	char	path[4096] = {0};
	char	*pwd = NULL;
	t_env	*curs = NULL;
	int		i = 0;

	sep = ft_strsplit(com, '/');
	if (sep[0][0] != '/' && sep[0][0] != '~') {
		if (flag == PHYSICAL)
			pwd = getcwd(pwd, PATH_MAX);
		else
			pwd = get_value_env("PWD", env);
		if (!pwd)
			pwd = getcwd(pwd, PATH_MAX);
		ft_strcat(path, pwd);
	}
	if (sep[0][0] == '~')
	{
		pwd = get_value_env("HOME", env);
		if (pwd)
			ft_strcat(path, pwd);
		i++;
	}
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
	curpath = ft_strdup(path);
	return (curpath);
}

int		sh_cd(char **com, t_env **env)
{
	char	*curpath;
	int		i;
	int		flag;

	curpath = NULL;
	flag = DEFAULT;
	i = 1;
	if (!com[1])
		curpath = get_value_env("HOME", env);
	else if (ft_strequ(com[1], "-")) {
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
	change_working_dir(curpath, env, com[i], flag);
	free(curpath);
	return (1);
}