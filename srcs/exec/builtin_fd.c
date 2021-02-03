#include "sh.h"
/* UNUSED  */
static char				*get_name(char *s)
{
	int				l;
	char			*name;
	int				i;
	int				c;

	i = 1;
	c = 0;
	l = ft_strlen(s);
	name = (char *)malloc(sizeof(char) * l);
	while (s[i])
		name[c++] = s[i++];
	name[c] = '\0';
	return (name);
}

static void			echo_env(char *s, t_env **env, int fd)
{
	char		*name;
	t_env		*cur;

	cur = find_env_variable(env, &s[1]);
//	name = get_name(s);
	ft_putstr_fd(cur->value, fd);
}

int			sh_echo(char **com, t_env **env, int fd)
{
	int			i;
	int			sp;
	int			n;

	i = 1;
	n = 0;
	sp = ft_strsplit_len(com) - 1;
	fd = 1;
	if (com[i] && ft_strequ(com[1], "-n"))
	{
		n = 1;
		i++;
	}
	while (com[i])
	{
		/*if (com[i][0] == '$')
			echo_env(com[i], env, fd);
		else
			print_echo(com[i], fd);*/
		ft_putstr_fd(com[i], fd);
		if (sp--)
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