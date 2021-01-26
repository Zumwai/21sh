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
static void			mis_cd(char *p, int sig)
{
	if (sig == 1)
	{
		ft_putstr_fd(p, 2);
		ft_putendl_fd(": No such file or directory", 2);
	}
	if (sig == 2)
		ft_putendl_fd("cd: Too many arguments.", 2);
	if (sig == 3)
		ft_putendl_fd(": No such file or directory.", 2);
	if (sig == 4)
		ft_putendl_fd("It`s nothing set for HOME.", 2);
}

static char			*get_post(char *s)
{
	int			i;
	int			j;
	int			l;
	char		*res;

	i = 0;
	j = 1;
	l = ft_strlen(s);
	res = (char *)malloc(sizeof(char) * l);
	while (s[j] != '\0')
	{
		res[i] = s[j];
		i++;
		j++;
	}
	res[i] = '\0';
	return (res);
}


static void			cd_rev(t_env **env)
{
	char		*pwd;
	char		*old_pwd;
	t_env		*cur;

	cur = find_env_variable(env, "PWD");
	pwd = ft_strdup(cur->value);
	cur = find_env_variable(env, "OLDPWD");
	old_pwd = ft_strdup(cur->value);
	if (!chdir(old_pwd))
	{
		sh_setnew("OLDPWD", pwd, env);
		sh_setnew("PWD", old_pwd, env);
		set_free_null(&pwd);
		set_free_null(&old_pwd);
	} else {
		//handle_cd_err
		handle_empty_error("CD", "chdir failed");
	}
	set_free_null(&pwd);
	set_free_null(&old_pwd);
}

static void			cd_home(t_env **env)
{
	t_env		*cur;
	char		*way;
	char		dir[256];

	way = NULL;
	getcwd(dir, 256);
	cur = find_env_variable(env, "HOME");
	way = ft_strdup(cur->value);
	sh_setnew("OLDPWD", dir, env);
	chdir(way);
	getcwd(dir, 256);
	sh_setnew("PWD", dir, env);
	free(way);
}

static void			cd_cd(char *way, t_env **env)
{
	char		dir[256];
	char		ndir[256];
	int			sig;

	getcwd(dir, 256);
	sig = chdir(way);
	if (sig < 0)
		mis_cd(NULL, 1);
	if (sig == 0)
	{
		getcwd(ndir, 256);
		sh_setnew("PWD", ndir, env);
		sh_setnew("OLDPWD", dir, env);
	}
}

static void			path_from_home(char *way, t_env **env)
{
	char		*way1;
	t_env		*cur;
	char		*post;

	post = get_post(way);
	way1 = NULL;
	if (!(cur = find_env_variable(env, "HOME")))
	{
		handle_empty_error("CD", "HOME variable is not defined");
	}
	if (!cur->value)
	{
		handle_empty_error("CD", "HOME variable is not defined");
	}
	else {
		way1 = ft_strdup(cur->value);
		ft_strcat(way1, post);
		cd_cd(way1, env);
		free(way1);
		free(post);
	}
}

int			sh_cd(char **cmd, t_env **env)
{
	if (cmd[1] == NULL)
		cd_home(env);
	else if (cmd[1][0] == '~' && cmd[1][1] == '\0')
		cd_home(env);
	else if (cmd[1][0] == '~' && cmd[1][1] != '\0')
		path_from_home(cmd[1], env);
	else if (cmd[1][0] == '-')
		cd_rev(env);
	else if (cmd[1][0] != '~' && cmd[1][0] != '-')
		cd_cd(cmd[1], env);
	else if (cmd[2] != NULL)
		mis_cd(NULL, 2);
	return 1;
}
*/

void	ft_concat(char *str, char **path, char *name)
{
	ft_strclr((*path));
	(*path) = strcpy((*path), str);
	(*path) = strcat((*path), "/");
	if (name)
		(*path) = strcat((*path), name);
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
	/*
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
	*/

	char		*pwd;
	char		*old_pwd;
	t_env		*cur;

	pwd = getcwd(pwd, PATH_MAX);
	if (!chdir(path))
	{
		sh_setnew("OLDPWD", pwd, env);
		sh_setnew("PWD", path, env);
	} else {
		//handle_cd_err
		handle_cd_err(check_rights(path, 1), com);
	}
//	set_free_null(&pwd);
	//set_free_null(&old_pwd);
}

int			sh_cd(char **com, t_env **ev)
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
