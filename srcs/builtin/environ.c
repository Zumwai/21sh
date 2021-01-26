#include "sh.h"

int					sh_setnew(char *nm, char *value, t_env **env)
{
	t_env	*curs;

	curs = *env;
	if (value == NULL)
		return (0);
	while (curs)
	{
		if (ft_strcmp(nm, curs->name) == 0)
		{
			free(curs->value);
			curs->value = ft_strdup(value);
			return (1);
		}
		if (curs->next == NULL)
		{
			if (!(curs->next = (t_env *)malloc(sizeof(t_env))))
				return (0);
			curs = curs->next;
			curs->name = ft_strdup(nm);
			curs->value = ft_strdup(value);
			curs->next = NULL;
			return (1);
		}
		curs = curs->next;
	}
	return (1);
}

extern int			sh_setenv(char **cmd, t_env **env)
{
	if (cmd[1] == NULL)
		display_env_list(cmd, env);
	else if (!cmd[3])
	{
		ft_putendl("setenv% TOO few arguments");
		ft_putendl("Usage: setenv <variable> <value>");
	}
	else if (!(sh_setnew(cmd[1], cmd[2], env)))
	{
		ft_putstr("setenv: nothing to set for this variable ");
		ft_putendl(cmd[1]);
	}
	return 1;
}

/* UNUSED */
static void				do_unset(char *nm, t_env **env)
{
	t_env			*tmp;
	t_env			*prev;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(nm, tmp->name) == 0)
		{
			set_free_null(&tmp->name);
			set_free_null(&tmp->value);
			if (tmp == *env)
				env = &(*env)->next;
			else
				prev->next = tmp->next;
			free(tmp);
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

/* UNUSED */
extern t_env				*sh_unset(char **nm, t_env **env)
{
	t_env			*cur;
	int				i;

	i = 1;
	cur = *env;
	while (nm[i] != NULL)
		do_unset(nm[i++], env);
	return (cur);
}