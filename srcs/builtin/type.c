/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 12:11:33 by aophion           #+#    #+#             */
/*   Updated: 2021/03/29 12:12:58 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int	compare_bultin(char *com)
{
	if (ft_strequ(com, "cd"))
		return (1);
	if (ft_strequ(com, "clear"))
		return (1);
	if (ft_strequ(com, "exit"))
		return (1);
	if (ft_strequ(com, "setenv"))
		return (1);
	if (ft_strequ(com, "unsetenv"))
		return (1);
	if (ft_strequ(com, "ppid"))
		return (1);
	if (ft_strequ(com, "pwd"))
		return (1);
	if (ft_strequ(com, "type"))
		return (1);
	if (ft_strequ(com, "set"))
		return (1);
	if (ft_strequ(com, "unset"))
		return (1);
	if (ft_strequ(com, "export"))
		return (1);
	return (0);
}

int			sh_type(char **com, t_env **env)
{
	char	*path;

	path = NULL;
	if (!com)
		return (1);
	if (!com[1])
		return (1);
	if (compare_bultin(com[1]))
		ft_printf("%s - is a shell builtin\n", com[1]);
	else if ((path = get_path(com[1], env)))
		ft_printf("%s - is %s\n", com[1], path);
	return (1);
}
