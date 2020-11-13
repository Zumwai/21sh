/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/29 14:46:02 by student           #+#    #+#             */
/*   Updated: 2020/07/29 15:36:45 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void		delete_env_list(t_env **ev)
{
	t_env	*curs;
	t_env	*tmp;

	curs = (*ev);
	while (curs)
	{
		tmp = curs->next;
		if (curs->name)
			free(curs->name);
		if (curs->value)
			free(curs->value);
		free(curs);
		curs = tmp;
	}
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
	tmp[2] = getcwd(buf, PATH_MAX);
	tmp[1] = OLDPWD;
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
