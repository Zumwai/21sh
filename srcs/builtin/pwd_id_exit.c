/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_id_exit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 12:45:12 by aophion           #+#    #+#             */
/*   Updated: 2021/03/29 12:48:23 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int		sh_exit(void)
{
	tcsetattr(STDIN_FILENO, TCSADRAIN, &g_sig.old);
	return (-1);
}

int		display_id_kid_parent(void)
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
	return (0);
}

int		sh_pwd(t_cmd *com, t_env **env, int fd)
{
	char	*curr;

	curr = NULL;
	curr = get_value_env("PWD", env);
	if (!curr)
		if (!(curr = getcwd(curr, PATH_MAX)))
			return (1);
	ft_putendl_fd(curr, fd);
	free(curr);
	return (0);
}
