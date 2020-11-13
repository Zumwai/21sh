/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shelp_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/24 12:06:23 by student           #+#    #+#             */
/*   Updated: 2020/07/29 22:15:44 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char	**ft_convert_back(t_env **ev)
{
	char	**tab;
	t_env	*curs;
	int		i;

	i = 0;
	curs = (*ev);
	while (curs)
	{
		curs = curs->next;
		i++;
	}
	tab = ft_newdim(i + 1);
	curs = (*ev);
	i = 0;
	while (curs)
	{
		tab[i] = ft_strnew(ft_strlen(curs->name) + ft_strlen(curs->value) + 1);
		ft_strclr(tab[i]);
		tab[i] = ft_strcpy(tab[i], curs->name);
		tab[i] = ft_strcat(tab[i], "=");
		tab[i] = ft_strcat(tab[i], curs->value);
		i++;
		curs = curs->next;
	}
	return (tab);
}

int			create_child_cmd(t_env **ev, char **av, char *path)
{
	int		f;
	int		status;
	char	**tmp;
	int		ret;

	tmp = ft_convert_back(ev);
	ret = 1;
	if ((f = fork()) == -1)
		handle_empty_error(av[0], "unable to fork");
	handle_all_signal(0);
	if (!f)
	{
		if ((execve(path, av, tmp)) == -1)
			ret = FIN;
	}
	else if (f > 0)
		waitpid(f, &status, 0);
	ft_free_tab(tmp);
	free(path);
	return (ret);
}
