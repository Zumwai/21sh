/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_err.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/24 20:00:54 by student           #+#    #+#             */
/*   Updated: 2020/07/29 22:09:31 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_empty_error(char *name, char *description)
{
	ft_putstr_fd("-shelp!: ", 2);
	ft_putstr_fd(name, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(description, 2);
}

int		handle_return_error(int num, char *name)
{
	ft_putstr_fd("-shelp!: ", 2);
	ft_putstr_fd(name, 2);
	if (num == -4 || num == -14)
		ft_putstr_fd(": Permission denied\n", 2);
	else if (num == -6)
		ft_putstr_fd(": Not an executable\n", 2);
	else if (num == -12 || num == -13)
		ft_putstr_fd(": No such file or directory\n", 2);
	return (num);
}

int		handle_cd_err(int num, char *name)
{
	ft_putstr_fd("-shelp!: ", 2);
	ft_putstr_fd(name, 2);
	if (num == -4)
		ft_putstr_fd(": Permission denied\n", 2);
	else if (num == -2 || num == -3)
		ft_putstr_fd(": No such file or directory\n", 2);
	else if (num == -7)
		ft_putstr_fd(": Not a directory\n", 2);
}
