/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 12:28:42 by aophion           #+#    #+#             */
/*   Updated: 2021/03/29 12:32:45 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int					display_env_list(char **com, t_env **ev, int scope)
{
	t_env	*cur;

	if (com)
		(void)com;
	cur = (*ev);
	while (cur)
	{
		if (cur->scope == scope)
		{
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
	/*int i;

	i = 0;
	while (cmd[i])
	{
        ft_putendl("zachem etot while v sh_setenv????");
        ft_putendl(cmd[i++]);
    }*/
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
	return (1);
}

extern int			sh_set(char **cmd, t_env **env)
{
	return (sh_setenv(cmd, env, 0));
}

extern int			sh_export(char **com, t_env **env)
{
	t_env	*curs;
	char	**set;
	int		i;

	i = 1;
	set = NULL;
	if (!com[1])
		return (1);
	while (com[i])
	{
		set = ft_strsplit(com[i], '=');
		sh_setnew(set[0], set[1], env, 1);
		i++;
		ft_free_tab(set);
	}
	return (1);
}
