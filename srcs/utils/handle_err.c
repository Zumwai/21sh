/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_err.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/24 20:00:54 by student           #+#    #+#             */
/*   Updated: 2021/03/27 20:20:35 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int     new_err_fd(t_cmd *cmd)
{
    int new;

    if (cmd->fd2 == 1)
    {
        new = 1;
        if (cmd->type == 6 || cmd->type == 7)
           new = get_fd_write(cmd);
    }
    if (cmd->fd2 == -1)
        new = open("/dev/NULL", O_CREAT | O_RDWR | O_TRUNC,
                   S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    return (new);
}

void	handle_empty_error(char *name, char *description)
{
	ft_putstr_fd("-shelp!: ", STDERR_FILENO);
	if (name)
		ft_putstr_fd(name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (description)
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
	else if (num == -7)
		ft_putstr_fd(": is a directory\n", STDERR_FILENO);
	else if (num == -2 || num == -3)
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	return (num);
}

void		handle_cd_err(int num, /*char *name*/t_cmd *cmd)
{
    int     fd;

    fd = 2;
    if (cmd->fd1 == 2)
        fd = new_err_fd(cmd);
	ft_putstr_fd("-shelp!: ", /*STDERR_FILENO*/ fd);
	ft_putstr_fd(/*name*/cmd->arr[1], /*STDERR_FILENO*/ fd);
	if (num == -4)
		ft_putstr_fd(": Permission denied\n", /*STDERR_FILENO*/ fd);
	else if (num == -2 || num == -3)
		ft_putstr_fd(": No such file or directory\n", /*STDERR_FILENO*/ fd);
	else if (num == -7)
		ft_putstr_fd(": Not a directory\n", /*STDERR_FILENO*/ fd);
}

void	handle_exit_errors(char *description)
{
	ft_putstr_fd("-shelp!: ", STDERR_FILENO);
	ft_putstr_fd(description, STDERR_FILENO);
	tcsetattr(STDIN_FILENO, TCSADRAIN, &g_sig.old);
	ft_putstr_fd("\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
}
