/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 12:27:12 by aophion           #+#    #+#             */
/*   Updated: 2021/03/29 12:31:11 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

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

extern int				sh_unset(char **nm, t_env **env, int fd)
{
	t_env			*cur;
	int				i;

	i = 1;
	cur = *env;
	while (nm[i] != NULL)
		do_unset(nm[i++], env);
	return (1);
}
