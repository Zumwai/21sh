/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shelp_ppid.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/29 14:18:34 by student           #+#    #+#             */
/*   Updated: 2020/07/29 14:19:18 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	display_id_kid_parent(void)
{
	pid_t	kid;
	pid_t	par;

	ft_putstr("Current process id: ");
	kid = (getpid());
	ft_putnbr(kid);
	ft_putchar('\n');
	ft_putstr("Current process parent id: ");
	par = (getppid());
	ft_putnbr(par);
	ft_putchar('\n');
}
