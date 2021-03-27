#include "sh.h"

int					sh_setnew(char *nm, char *value, t_env **env, int scope)
{
	t_env	*curs;

	curs = *env;
	if (value == NULL)
		return (0);
	if (!nm)
		return (0);
	while (curs)
	{
		if (ft_strcmp(nm, curs->name) == 0)
		{
			if (ft_strlen(value) < MAX_ARG_STRLEN) {
				curs->scope = scope;
				if (curs->value)
					free(curs->value);
				curs->value = NULL;
				if (value)
					curs->value = ft_strdup(value);
			} else {
				handle_empty_error(value, "variable length if too long");
			}
			return (1);
		}
		else if (curs->next == NULL)
		{
			if (ft_strlen (nm) < MAX_ARG_STRLEN && ft_strlen(value) < MAX_ARG_STRLEN)
			{
				if (!(curs->next = (t_env *)malloc(sizeof(t_env))))
					return (0);
				curs = curs->next;
				if (nm)
					curs->name = ft_strdup(nm);
				if (value)
					curs->value = ft_strdup(value);
				curs->scope = scope;
				curs->next = NULL;
			}
			else {
				handle_empty_error(value, "variable length if too long");
			}
			return (1);
		}
		curs = curs->next;
	}
	return (1);
}

int				display_env_list(char **com, t_env **ev, int scope)
{
	t_env	*cur;

	if (com)
		(void)com;
	cur = (*ev);
	while (cur)
	{
		if (cur->scope == scope) {
			ft_putstr(cur->name);
			ft_putchar('=');
			ft_putstr(cur->value);
			ft_putchar('\n');
		}
		cur = cur->next;
	}
	return (1);
}

extern int			sh_setenv(char **cmd, t_env **env, int scope)
{
	int i = 0;

	while (cmd[i])
		ft_putendl(cmd[i++]);
	if (cmd[1] == NULL)
		display_env_list(cmd, env, scope);
	else if (ft_strsplit_len(cmd) > 3)
	{
		ft_putendl("setenv% TOO few arguments");
		ft_putendl("Usage: setenv <variable> <value>");
	}
	else if (ft_strsplit_len(cmd) < 3)
	{
		ft_putstr("setenv: nothing to set for this variable ");
		ft_putendl(cmd[1]);
	}
	else
		sh_setnew(cmd[1], cmd[2], env, scope);
	return 1;
}

extern int		sh_set(char **cmd, t_env **env)
{
	return (sh_setenv(cmd, env, 0));
}

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


extern int				sh_unset(char **nm, t_env **env, __attribute((unused))int fd)
{
	t_env			*cur;
	int				i;

	i = 1;
	cur = *env;
	while (nm[i] != NULL)
		do_unset(nm[i++], env);
	return (1);
}

extern int				sh_export(char **com, t_env **env)
{
	t_env *curs;
	char	**set;
	int		i = 1;
	set = NULL;
	if (!com[1])
		return 1;
	while (com[i])
	{
		set = ft_strsplit(com[i], '=');
		sh_setnew(set[0], set[1], env, 1);
		i++;
		ft_free_tab(set);
	}
	return 1;
}