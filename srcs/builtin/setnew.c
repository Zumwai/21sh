/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setnew.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 12:44:29 by aophion           #+#    #+#             */
/*   Updated: 2021/03/29 12:44:40 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int			set_existing_env(t_env *curs,
						char *nm, char *value, int scope)
{
	if (ft_strlen(value) < MAX_ARG_STRLEN)
	{
		curs->scope = scope;
		if (curs->value)
			free(curs->value);
		curs->value = NULL;
		if (value)
			curs->value = ft_strdup(value);
	}
	else
	{
		handle_empty_error(value, "variable length if too long");
		return (1);
	}
	return (0);
}

static int			set_new_env(t_env *curs, char *nm, char *value, int scope)
{
	if (ft_strlen(nm) < NAME_MAX && ft_strlen(value) < MAX_ARG_STRLEN)
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
	else
	{
		handle_empty_error(value, "variable length if too long");
		return (1);
	}
	return (0);
}

int					sh_setnew(char *nm, char *value, t_env **env, int scope)
{
	t_env	*curs;

	curs = *env;
	if (value == NULL)
		return (1);
	if (!nm)
		return (1);
	while (curs)
	{
		if (ft_strcmp(nm, curs->name) == 0)
		{
			return (set_existing_env(curs, nm, value, scope));
		}
		else if (curs->next == NULL)
		{
			return (set_new_env(curs, nm, value, scope));
		}
		curs = curs->next;
	}
	return (0);
}
