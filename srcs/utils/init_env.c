/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 19:39:31 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 20:06:13 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

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
	tmp->scope = 1;
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
	tmp[1] = PWD;
	sh_setnew(tmp[1], tmp[2], ev, 1);
	free(tmp);
}

static void	set_pid_env(t_env **ev)
{
	char	*pid;
	int		kid;

	kid = getpid();
	pid = ft_itoa(kid);
	sh_setnew("$", pid, ev, 0);
	free(pid);
}

t_env		*create_env_list(char **env)
{
	t_env	*tmp;
	t_env	*head;
	int		i;

	i = -1;
	tmp = NULL;
	if (!(*env))
		return (NULL);
	while (env[++i])
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
	}
	set_dir_env(&head);
	set_pid_env(&head);
	return (head);
}
