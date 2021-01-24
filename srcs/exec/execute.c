#include "sh.h"

t_env				*clear_list(t_env *env)
{
	t_env			*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->name);
		tmp->name = NULL;
		free(tmp->value);
		tmp->value = NULL;
		free(tmp);
		tmp = NULL;
	}
	free(env);
	return (NULL);
}

int					is_it_avalible(char *s)
{
	if (access(s, F_OK) == -1)
	{
		ft_putendl("Unavalible command.");
		return (-1);
	}
	return (1);
}

void				only_sp(char **line, char target, char change)
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

char				**get_ways(t_env *env)
{
	char			**res;
	char			*temp;

	res = NULL;
	while (env != NULL)
	{
		if (ft_strcmp("PATH", env->name) == 0)
		{
			temp = ft_strdup(env->value);
			//only_sp(&temp, ' ', ':');
			res = ft_strsplit(temp, ':');
			free(temp);
			break ;
		}
		env = env->next;
	}
	return (res);
}

char			*it_path(char *s, t_env *env)
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
		}
	}
	ft_strsplit_free(&way);
	return (buf);
}

char				*get_path(char *s)
{
	t_env			*env;
	char			*res;

	env = env_list();
	res = it_path(s, env);
	clear_list(env);
	if (is_it_avalible(res))
	{
		return (res);
	}
	free(res);
	return (NULL);
}

int					command(char *s)
{
	if (/*(ft_strcmp(s, ECHO) == 0) ||
		(ft_strcmp(s, ENV) == 0) || */(ft_strcmp(s, SETENV) == 0) ||
		(ft_strcmp(s, UNSETENV) == 0) || (ft_strcmp(s, CD) == 0) ||
		(ft_strcmp(s, EXIT) == 0) || (ft_strcmp(s, "clear") == 0))
		return (1);
	else
		return (0);
}

static void terminate_child(char *command)
{
	ft_putstr_fd("Execve failed to execute ", STDERR_FILENO);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd("\nProcess returned code -1\n", STDERR_FILENO);
	exit(-1);
}

static char	**convert_env_array(t_env **ev)
{
	char	**tab;
	t_env	*curs;
	int		i;

	i = 0;
	curs = (*ev);
	while (curs)
	{
		curs = curs->next;
		i++;
	}
	tab = ft_newdim(i + 1);
	curs = (*ev);
	i = 0;
	while (curs)
	{
		tab[i] = ft_strnew(ft_strlen(curs->name) + ft_strlen(curs->value) + 1);
		ft_strclr(tab[i]);
		tab[i] = ft_strcpy(tab[i], curs->name);
		tab[i] = ft_strcat(tab[i], "=");
		tab[i] = ft_strcat(tab[i], curs->value);
		i++;
		curs = curs->next;
	}
	return (tab);
}

void			do_proc(int read, int fd, char *path, t_cmd *cmd, t_env **env)
{
	pid_t		pid;
	char		**environ;

	environ = convert_env_array(env);
	if ((pid = fork()) == 0)
	{
		if (read != 0)
		{
			dup2(read, 0);
			close(read);
		}
		if (fd != 1 && cmd->type == 2)
		{
			dup2(fd, 1);
			close(fd);
		}
		if (execve(path, cmd->arr, environ) == -1)
			terminate_child(path);
		//// как-то завернуть процесс на выход
	}
	else
		wait(&pid);
	free(cmd->target);
	ft_free_tab(&environ);
}

void			do_target(t_cmd *cmd)
{
	while (cmd)
	{
		if ((!command(cmd->arr[0]) && cmd->arr[0][0] != '/') ||
			(ft_strcmp("env", cmd->arr[0]) == 0 || ft_strcmp("echo", cmd->arr[0]) == 0))
		{
			cmd->target = get_path(cmd->arr[0]);
			if (cmd->target == NULL)
				return;
		}
		cmd = cmd->next;
	}
}

int			execute(t_cmd *cmd, t_env **env)
{
	int			read;
	int			fd[2];
	pid_t		pid;
	t_cmd 		*head;
	int			res;

	res = 1;
	head = cmd;
	read = 0;
	do_target(cmd);
	while (cmd)
	{
		if ((pid = fork()) == 0)
		{
			pipe(fd);
			if (command(cmd->arr[0]) && cmd->type != 2)
				res = do_builtin(cmd, env);
			else
				do_proc(read, fd[1], cmd->target, cmd, env);
			close(fd[1]);
			read = fd[0];
		}
		else
			wait(&pid);
		cmd = cmd->next;
	}
	//free_cmd(head);
	return (res);
}

