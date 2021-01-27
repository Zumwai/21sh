#include "sh.h"

static	int					command(char *s)
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
//		ft_free_tab(&environ);
		//// как-то завернуть процесс на выход
	}
	else
		wait(&pid);
	set_free_null(&cmd->target);
	ft_free_tab(&environ);
}
/* UNUSED
static void			do_target(t_cmd *cmd, t_env **env)
{
	while (cmd)
	{
		if ((!command(cmd->arr[0]) && cmd->arr[0][0] != '/') ||
			(ft_strcmp("env", cmd->arr[0]) == 0 || ft_strcmp("echo", cmd->arr[0]) == 0))
		{
			cmd->target = get_path(cmd->arr[0], env);
			if (cmd->target == NULL)
				return;
		}
		cmd = cmd->next;
	}
}
*/
static void *get_builtin(char *com)
{

	if (ft_strequ(com, "cd"))
		return (&sh_cd);
	if (ft_strequ(com, "clear"))
		return(&sh_clear);
	if (ft_strequ(com, "echo"))
		return(&sh_echo);
	if (ft_strequ(com, "exit"))
		return(&sh_exit);
	if (ft_strequ(com, "clear"))
		return(&sh_clear);
	if (ft_strequ(com, "setenv"))
		return(&sh_setenv);
//	if (ft_strequ(com, "unsetenv"))
//		return(&sh_unset);
	if (ft_strequ(com, "ppid"))
		return (&display_id_kid_parent);
	if (ft_strequ(com, "pwd"))
		return (&sh_pwd);
	return NULL;
}
int			execute(t_cmd *cmd, t_env **env)
{
	int			read;
	int			fd[2];
	pid_t		pid;
	t_cmd 		*head;
	int		(*builtin)();

	int			
	res = 1;
	head = cmd;
	read = 0;
	builtin = NULL;
	while (cmd)
	{
		//if ((pid = fork()) == 0)
		//{
			pipe(fd);
			if (cmd->type != 2 && ((builtin = get_builtin(cmd->arr[0]))))
			{
				res = builtin(cmd->arr, env, fd);
			//	exit(1);
			}
			else {
				cmd->target = get_path(cmd->arr[0], env);
				if (cmd->target != NULL)
					do_proc(read, fd[1], cmd->target, cmd, env);
			}
			close(fd[1]);
			read = fd[0];
		//}
		//else
			//wait(&pid);
		cmd = cmd->next;
	}
	//free_cmd(head);
	return (res);
}

