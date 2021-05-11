#include "sh.h"
#include <stdio.h>

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
	if (MAX_ARG_STRINGS <= i)
	{
		handle_empty_error("env list is too long", "execve limit");
		return NULL;
	}
	tab = ft_newdim(i + 1);
	curs = (*ev);
	i = 0;
	while (curs)
	{
		if (curs->scope == 1) {
			tab[i] = ft_strnew(ft_strlen(curs->name) + ft_strlen(curs->value) + 1);
			ft_strclr(tab[i]);
			tab[i] = ft_strcpy(tab[i], curs->name);
			tab[i] = ft_strcat(tab[i], "=");
			tab[i] = ft_strcat(tab[i], curs->value);
			i++;
		}
		curs = curs->next;
	}
	return (tab);
}

int				next_two(t_cmd * cmd)
{
	while (cmd && (cmd->type == 8 || cmd->type == 9))
		cmd = cmd->next;
	if (cmd->type == 2)
		return (1);
	return (0);
}

int             new_err_fd_fork(t_cmd *cmd, int fd)
{
    int         err;

    if (cmd->fd2 == -1)
    {
        err = open("/dev/null", O_CREAT | O_RDWR | O_TRUNC,
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    }
    if (cmd->fd2 == 1)
        err = fd;
    return (err);
}

void			do_proc(int read, int fd, char *path, t_cmd *cmd, t_env **env)
{
	pid_t		pid;
	char		**environ;
	int         err_fd;

	err_fd = 2;
	if (!(environ = convert_env_array(env)))
		return ;
	if ((pid = fork()) == 0)
	{
		handle_all_signals(0);
        if (cmd->fd1 == 2)
        {
            err_fd = new_err_fd_fork(cmd, fd);
            dup2(err_fd, 2);
        }
		if (read != 0)
		{
			dup2(read, 0);
			close(read);
		}
		if (fd != 1 && (cmd->type == 2 || cmd->type == 6 || cmd->type == 7  ||
		((cmd->type == 8 || cmd->type == 9) && next_two(cmd))))
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
	if (read != 0)
		close(read);
	if (fd != 1 && fd != 2)
		close(fd);
	handle_all_signals(1);
	set_free_null(&cmd->target);
	ft_free_tab(environ);
	environ = NULL;
}

static int	check_isbuiltin(char *com)
{
	if (ft_strequ(com, "cd"))
		return 1;
	if (ft_strequ(com, "echo"))
		return 2;
	if (ft_strequ(com, "exit"))
		return 3;
	if (ft_strequ(com, "setenv"))
		return 4;
	if (ft_strequ(com, "unset"))
		return 5;
	if (ft_strequ(com, "ppid"))
		return 6;
	if (ft_strequ(com, "type"))
		return 7;
	if (ft_strequ(com, "pwd"))
		return 8;
	if (ft_strequ(com, "set"))
		return 9;
	if (ft_strequ(com, "export"))
		return 10;
	return 0;
}

int	exec_builtin(t_cmd *com, t_env **env, int fd, int num)
{
	if (num == 1)
		return (sh_cd(com, env));
	if (num == 2)
		return (sh_echo(com, env, fd, 1));
	if (num == 3)
		return (sh_exit());
	if (num == 4)
		return (sh_setenv(com->arr, env, 1));
	if (num == 5)
		return (sh_unset(com->arr, env, fd));
	if (num == 6)
		return (display_id_kid_parent());
	if (num == 7)
		return (sh_type(com, env));
	if (num == 8)
		return (sh_pwd(com, env, fd));
	if (num == 9)
		return (sh_set(com, env));
	if (num == 10)
		return (sh_export(com, env));
	return (1);
}

int             get_fd_write(t_cmd *cmd)
{
    int         fd;
    t_cmd       *cur;

    fd = 0;
    cur = cmd;
    while ((cur->type == 6 || cur->type == 7 || cur->type == 8 || cur->type == 9) && cur->next)
    {
        if (cur->type == 7)
            fd = open(cur->next->arr[0], O_CREAT | O_RDWR | O_APPEND,
                      S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
        if (cur->type == 6)
            fd = open(cur->next->arr[0], O_CREAT | O_RDWR | O_TRUNC,
                      S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
        if (cur->type == 8 || cur->type == 9)
            fd = open(cur->next->arr[0], O_RDONLY);
        if (cur->next->type == 6 || cur->next->type == 7)
            close(fd);
        cur = cur->next;
    }
    return (fd);
}

static int 		get_cmd_type(t_cmd *cmd, int fd)
{
	while (cmd && (cmd->type == 8 || cmd->type == 9))
		cmd = cmd->next;
	if (cmd->type == 2)
		return (fd);
	return (1);
}

void            print(t_cmd *c)
{
    while (c)
    {
        ft_putendl(c->arr[0]);
        c = c->next;
    }
}

void		what_about_file(t_cmd *cmd)
{
	t_cmd 	*cur;
	struct stat	buf;
	int 			fd;

	cur = cmd;
	if (cur->type == 2)
	{
		while (cur->type == 2)
			cur = cur->next;
	}
	if (cur->type == 6 || cur->type == 7)
	{
		while (cur->type == 6 || cur->type == 7)
			cur = cur->next;
		if (stat(cur->arr[0], &buf) == -1)
		{
			fd = open(cur->arr[0], O_CREAT | O_RDWR | O_APPEND,
					  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
			close(fd);
		}
	}
}

int        prepare_to(t_cmd *cmd)
{
    int    res;

    res = 1;
    if (cmd->fd1 == 1)
       return (res);
    if (cmd->type == 6 || cmd->type == 7 || cmd->type == 2)
        what_about_file(cmd);
    if (cmd->type == 6 || cmd->type == 7)
        res = get_fd_write(cmd);
    return (res);
}

int         last_check(t_cmd * cmd, int fd)
{
    int     tmp;
    if (cmd->fd1 == 1 && cmd->fd2 == 2)
        return (2);
    if (cmd->fd1 == 1 && cmd->fd2 == -1)
    {
        tmp = open("/dev/NULL", O_CREAT | O_RDWR | O_TRUNC,
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
        return (tmp);
    }
    return (fd);
}

int         need_pipe(int p)
{
    if (p == 6 || p == 7 || p == 8 || p == 9 || p == 2)
        return (1);
    return (0);
}

int			execute(t_cmd *cmd, t_env **env, t_yank *buf)
{
	int			read;
	int			fd[2];
	int		builtin;
	int ffd;
	int err_fd;

	err_fd = 0;
	fd[0] = 0;
	fd[1] = 1;
	int res;
	res = 1;
	read = 0;
	builtin = 0;
	ffd = 1;
	if (!cmd->arr || !cmd->arr[0])
		return 1;
	while (cmd)
	{
	    ffd = prepare_to(cmd);
	    if ((builtin = check_isbuiltin(cmd->arr[0])) != 0)
	    {
	        if (cmd->type == 2)
	        {
	            pipe(fd);
	            ffd = last_check(cmd, fd[1]);
	            res = exec_builtin(cmd, env, ffd, builtin);
	        }
	        else
	            {
                    ffd = last_check(cmd, ffd);
                    res = exec_builtin(cmd, env, ffd, builtin);
                }
                if (ffd != 1)
                    close(ffd);
            }
           else
               {
                cmd->target = get_path(cmd->arr[0], env);
                if (cmd->target != NULL)
                {
                    if (need_pipe(cmd->type))
                        pipe(fd);
                    if (cmd->type == 2)
                    {
                        ffd = last_check(cmd, fd[1]);
                        do_proc(read, ffd, cmd->target, cmd, env);
                        close(ffd);
                    }
                    else if (cmd->type == 1 || cmd->type == 0)
                        do_proc(read, ffd, cmd->target, cmd, env);
                    else if (cmd->type == 6 || cmd->type == 7)
                    {
                        ffd = last_check(cmd, ffd);
                        dup2(ffd, fd[1]);
                        do_proc(read, ffd, cmd->target, cmd, env);
                        close(ffd);
                    }
                    else if ((cmd->type == 8 || cmd->type == 9) || (cmd->prev->type == 8 || cmd->prev->type == 9))
                    {
                        read = get_fd_write(cmd);
                        ffd = get_cmd_type(cmd, fd[1]);
                        do_proc(read, ffd, cmd->target, cmd, env);
                    }
                }
            }
            if (cmd->type == 6 || cmd->type == 7 || cmd->type == 8 || cmd->type == 9)
            {
                while (cmd->next && (cmd->type == 6 || cmd->type == 7 || cmd->type == 8 || cmd->type == 9))
                    cmd = cmd->next;
            }
            if (cmd->type == 2)
                read = fd[0];
            cmd = cmd->next;
        }
	if (read != 0)
		close(read);
	if (fd[0] != 0)
		close(fd[0]);
	if (fd[1] != 1)
		close(fd[1]);
	return (res);
}