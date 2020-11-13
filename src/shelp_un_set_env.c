/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shelp_un_set_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/29 15:02:53 by student           #+#    #+#             */
/*   Updated: 2020/07/30 12:09:35 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				unset_env(char **com, t_env **ev)
{
	t_env	*curr;
	t_env	*del;

	del = NULL;
	if (com[1] && (del = find_env(ev, com[1])))
	{
		curr = (*ev);
		while (curr && curr->next != del)
			curr = curr->next;
		if (!curr)
			return (-1);
		curr->next = curr->next->next;
		free(del->name);
		free(del->value);
		free(del);
	}
	return (1);
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

static int		ft_find_exist(t_env **ev, char **com)
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

int				set_env(char **com, t_env **ev)
{
	if (!com[1])
		return (display_env_list(com, ev));
	if (!(ft_isletter(com[1][0])))
	{
		handle_empty_error(com[1], ": Variable name should start with a letter\n");
		return (1);
	}
	return (ft_find_exist(ev, com));
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
