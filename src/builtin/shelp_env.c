/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shelp_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/27 11:36:40 by student           #+#    #+#             */
/*   Updated: 2020/07/30 12:09:10 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

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

static int			modify_env_var(char **com, t_env **ev, int j)
{
	int		i;
	t_env	*cur;
	t_env	*tmp;

	i = ft_strchr(com[j], '=') - com[j];
	cur = (*ev);
	while (cur)
	{
		tmp = cur;
		if (ft_strnequ(cur->name, com[j], i))
		{
			free(cur->value);
			cur->value = ft_strdup(&com[j][i + 1]);
			return (1);
		}
		cur = cur->next;
		if (!cur)
		{
			com[j][i] = '\0';
			tmp->next = ft_last_env(com[j], &com[j][i + 1]);
		}
	}
	return (1);
}

static int			validate_env_com(char *str)
{
	char	*tmp;

	if (!str)
		return (0);
	tmp = ft_strchr(str, '=');
	if (!tmp)
		return (0);
	if (tmp == str)
		return (0);
	if (tmp[1] == '=')
		return (0);
	if (str[0] == '$')
		return (0);
	return (1);
}

static t_env			*clone_env(t_env **ev)
{
	t_env	*mod;
	t_env	*head;
	t_env	*cur;

	if (!(head = (t_env *)malloc(sizeof(t_env))))
		return (NULL);
	head->name = ft_strdup((*ev)->name);
	head->value = ft_strdup((*ev)->value);
	mod = head;
	cur = (*ev)->next;
	while (cur)
	{
		if (!(mod->next = (t_env *)malloc(sizeof(t_env))))
			return (NULL);
		mod = mod->next;
		mod->next = NULL;
		mod->name = ft_strdup((cur)->name);
		mod->value = ft_strdup((cur)->value);
		cur = cur->next;
	}
	return (head);
}

int					manage_env(char **com, t_env **ev)
{
	t_env	*new;
	int		i;

	i = 1;
	if (!com[i])
		return (display_env_list(com, ev));
	if (com[i])
	{
		new = clone_env(ev);
		while (validate_env_com(com[i]))
		{
			modify_env_var(com, &new, i);
			i++;
		}
		if (com[i])
		{
			i = go_do(&com[i], &new);
			delete_env_list(&new);
			return (i);
		}
		else
			delete_env_list(&new);
	}
	return (1);
}
