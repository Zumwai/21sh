/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shelp_echo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/24 10:07:48 by student           #+#    #+#             */
/*   Updated: 2020/07/29 22:50:33 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int		ft_echo_env(char *av, t_env **ev)
{
	while (!ft_strequ((*ev)->name, av) && (*ev)->next)
		ev = &((*ev)->next);
	if (!ft_strequ((*ev)->name, av))
		return (1);
	else
		ft_putstr((*ev)->value);
	return (0);
}

static void		ft_print_echo(char **av, int i, t_env **ev)
{
	int		sp;
	int		j;

	sp = 0;
	while (av[i])
	{
		j = 0;
		if (av[i][0] == '$')
			sp = ft_echo_env(&av[i][1], ev);
		else
		{
			while (av[i][j])
			{
				if (av[i][j] != '"')
					write(1, &av[i][j], 1);
				j++;
			}
		}
		if (!sp && av[i + 1])
			ft_putchar(' ');
		i++;
	}
}

int				ft_echo(char **av, t_env **ev)
{
	int		i;
	int		n;

	n = 0;
	i = 1;
	if (av[1] && ft_strequ(av[1], "-n"))
	{
		n = 1;
		i++;
	}
	if (av[i])
		ft_print_echo(av, i, ev);
	if (!n)
		ft_putchar('\n');
	return (1);
}
