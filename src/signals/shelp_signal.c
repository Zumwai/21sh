/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shelp_signal.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/24 11:53:05 by student           #+#    #+#             */
/*   Updated: 2020/07/29 14:29:21 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	handle_child_signal(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr("\n");
		signal(SIGINT, handle_child_signal);
	}
}

void	handle_main_signal(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr("\n");
		ft_putstr("shelp$>");
		signal(SIGINT, handle_main_signal);
	}
}

void	handle_all_signal(int pid)
{
	if (pid)
		signal(SIGINT, handle_main_signal);
	if (!pid)
		signal(SIGINT, handle_child_signal);
}
