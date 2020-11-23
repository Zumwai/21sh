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

#include "shell.h"

void	handle_empty_error(char *name, char *description)
{
	ft_putstr_fd("-shelp!: ", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(description, STDERR_FILENO);
}

int		handle_return_error(int num, char *name)
{
	ft_putstr_fd("-shelp!: ", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	if (num == -4)
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	else if (num == -6)
		ft_putstr_fd(": Not an executable\n", STDERR_FILENO);
	else if (num == -2 || num == -3)
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	return (num);
}

void		handle_cd_err(int num, char *name)
{
	ft_putstr_fd("-shelp!: ", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	if (num == -4)
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	else if (num == -2 || num == -3)
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	else if (num == -7)
		ft_putstr_fd(": Not a directory\n", STDERR_FILENO);
}

void	handle_exit_errors(char *description)
{
	ft_putstr_fd("ft_select: ", STDERR_FILENO);
	ft_putstr_fd(description, STDERR_FILENO);
//	reset_tty();
	exit(EXIT_FAILURE);
}
