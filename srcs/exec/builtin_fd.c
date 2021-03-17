#include "sh.h"
#include <stdio.h>

static void			echo_env(char *s, t_env **env, int fd)
{
	char		*name;
	t_env		*cur;

	cur = find_env_variable(env, &s[1]);
	ft_putstr_fd(cur->value, fd);
}

int			sh_echo(char **com, t_env **env, int fd)
{
	int			i;
	int			n;

	i = 1;
	n = 0;
	if (com[i] && ft_strequ(com[1], "-n"))
	{
		n = 1;
		i++;
	}
	while (com[i])
	{
		ft_putstr_fd(com[i], fd);
		if (com[i + 1])
			ft_putchar_fd(' ', fd);
		i++;
	}
	if (!n)
		write(fd, "\n", 1);
	return 1;
}

int			sh_env(char **com, t_env **env, __attribute((unused))int fd)
{
	int			i;
	t_env		*curs;


	i = 0;
	curs = *env;
	while (curs)
	{
		ft_putendl_fd(curs->name, fd);
		ft_putendl_fd("=", fd);
		ft_putendl_fd(curs->value, fd);
		curs = curs->next;
	}
	return 1;
}

int				sh_clear(__attribute__((unused))char **com, __attribute__((unused))t_env **env)
{
	write(1, " \e[1;1H\e[2J", 12);
	return 1;
}

int				do_builtin()
{
	return 1;
}