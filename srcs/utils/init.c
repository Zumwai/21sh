#include "sh.h"

t_cmd			*init_cmd(void)
{
	t_cmd		*new;

	if (!(new = (t_cmd *)malloc(sizeof(t_cmd))))
		handle_exit_errors("Malloc returned NULL");
	ft_memset(new, 0, sizeof(t_cmd));
	/*
	new->arr = NULL;
	new->fd1 = -1;
	new->fd2 = -1;
	new->type = -1;
	new->r_type = -1;
	new->target = NULL;
	new->next = NULL;
	new->prev = NULL;
	*/
	return (new);
}

t_cmd			*new_cmd(t_cmd *prev)
{
	t_cmd		*new;

	new = init_cmd();
	new->prev = prev;
	return (new);
}

static void		init_tty(void)
{
	int		res;
	char	*tty_name;

	if (read(STDIN_FILENO, NULL, 0) == -1) /* useless */
		handle_exit_errors("Shelp must have access to STDIN");
	if (!(tty_name = getenv("TERM")))
		handle_exit_errors("terminal type is not defined\n");
	if (!isatty(STDIN_FILENO))
		handle_exit_errors("should be run in a terminal\n");
	res = tgetent(NULL, tty_name);
	if (res < 0)
		handle_exit_errors("could not access the termcap data base\n");
	if (!res)
		handle_exit_errors("specify a valid terminal name with setenv\n");
}

static void init_tty_attr(t_yank *buf)
{
	struct termios	old_tty = {0};
	struct termios	tty = {0};

	tcgetattr(STDIN_FILENO, &old_tty);
	tcgetattr(STDIN_FILENO, &tty);
	//tty.c_lflag &= ~(ECHO | ICANON);
	tty.c_lflag &= ~(ECHO | ICANON | ISIG);
	//tty.c_oflag &= ~(TABDLY);
//	tty.c_lflag &= ~(ECHO | IEXTEN | ISIG);
	tty.c_cc[VMIN] = 0;
	tty.c_cc[VTIME] = 1;
	buf->old = old_tty;
	buf->work= tty;
	tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

static t_yank	*init_buffer(void)
{
	t_yank *new;

	if(!(new = (t_yank *)malloc(sizeof(t_yank))))
		return (NULL);
	ft_memset(new, 0, sizeof(t_yank));
	init_tty_attr(new);
	 /*
	new->size = 0;
	new->yanked = 0;
	new->history = NULL;
	new->current = NULL;
	new->saved = NULL;
	new->hist_ptr = NULL;
	new->trie = NULL;
	new->counter = 0;
	new->sub = NULL; */
	return (new);
}

t_env			*ft_last_env(char *name, char *value)
{
	t_env	*tmp;

	tmp = NULL;
	if (!(tmp = (t_env *)malloc(sizeof(t_env))))
		return (NULL);
	if (!(tmp->name = ft_strdup(name)))
		return (NULL);
	if (!value)
		tmp->value = NULL;
	else
	{
		if (!(tmp->value = ft_strdup(value)))
			return (NULL);
	}
	tmp->next = NULL;
	return (tmp);
}

int				display_env_list(char **com, t_env **ev)
{
	t_env	*cur;

	if (com)
		(void)com;
	cur = (*ev);
	while (cur)
	{
		ft_putstr(cur->name);
		ft_putchar('=');
		ft_putstr(cur->value);
		ft_putchar('\n');
		cur = cur->next;
	}
	return (1);
}

int		find_exitsing_env(t_env **ev, char **com)
{
	t_env	*curr;
	t_env	*tmp;

	curr = (*ev);
	while (curr)
	{
		tmp = curr;
		if (ft_strequ(curr->name, com[1]))
		{
			free(curr->value);
			if (com[2])
				curr->value = ft_strdup(com[2]);
			else
				curr->value = NULL;
			return (1);
		}
		curr = curr->next;
	}
	tmp->next = ft_last_env(com[1], com[2]);
	return (1);
}

t_env	*find_env(t_env **ev, char *name)
{
	t_env	*curs;

	curs = (*ev);
	while (curs)
	{
		if (ft_strequ(curs->name, name))
			return (curs);
		curs = curs->next;
	}
	return (NULL);
}

int				set_env(char **com, t_env **ev)
{
	if (!com[1])
		return (display_env_list(com, ev));
	if (!(ft_isletter(com[1][0])))
	{
		handle_empty_error(com[1], ": Variable name should start with a letter\n");
		return (1);
	}
	if (ft_strlen(com[1]) > 255) {
		handle_empty_error(com[1], ": Variable name is too long");
		return (1);
	}
	return (find_exitsing_env(ev, com));
}

t_env		*add_env(char *env)
{
	int		c;
	t_env	*tmp;

	c = 0;
	if (!(tmp = (t_env *)malloc(sizeof(t_env))))
		return (NULL);
	while (env[c] != '=')
		c++;
	if (!(tmp->name = ft_strnew(c)))
		return (NULL);
	tmp->name = ft_strncpy(tmp->name, env, c);
	tmp->value = ft_strdup(&env[c + 1]);
	tmp->next = NULL;
	return (tmp);
}

static void	set_dir_env(t_env **ev)
{
	char	**tmp;
	char	buf[PATH_MAX];

	tmp = ft_newdim(4);
	tmp[0] = NULL;
	tmp[1] = "OLDPWD";
	tmp[2] = getcwd(buf, PATH_MAX);
	set_env(tmp, ev);
	tmp[1] = PWD;
	set_env(tmp, ev);
	free(tmp);
}

t_env		*create_env_list(char **env)
{
	t_env	*tmp;
	t_env	*head;
	int		i;

	i = 0;
	tmp = NULL;
	if (!env)
		return (NULL);
	if (!(*env))
		return (NULL);
	while (env[i])
	{
		if (!tmp)
		{
			tmp = add_env(env[i]);
			head = tmp;
		}
		else
		{
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = add_env(env[i]);
		}
		i++;
	}
	set_dir_env(&head);
	return (head);
}

t_env	*init_shell(int ac, char **av, char **env, t_yank **buffer)
{
	t_env	*ev;

	if (ac < 1)
		handle_exit_errors("Wrong number of arguments");
	if (!av[0])
		handle_exit_errors("Wrong number of arguments");
	init_tty();
	if (!(ev = create_env_list(env)))
		handle_exit_errors("Failed to crate ENV list");
	if (!(*buffer = init_buffer()))
		handle_exit_errors("Malloc returned NULL");
	return (ev);
}