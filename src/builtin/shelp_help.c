/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shelp_help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/29 15:08:10 by student           #+#    #+#             */
/*   Updated: 2020/07/30 12:02:11 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		ft_help(void)
{
	ft_putendl("\nShelp version 0.1");
	ft_putendl("Available built-in functions:");
	ft_putendl(" cd [path];");
	ft_putendl(" echo [$(variable_name)] or [..text..];");
	ft_putendl(" setenv [variable_name] [variable_value];");
	ft_putendl(" unsetenv [variable_name];");
	ft_putendl(" env [[variable_name]=[variable_value]] [command_name];");
	ft_putendl(" printenv;");
	ft_putendl(" ppid;");
	ft_putendl(" pwd;");
	ft_putendl(" exit;\n");
	return (1);
}

int		ft_pwd(void)
{
	char	*curr;

	curr = NULL;
	if (!(curr = getcwd(curr, PATH_MAX)))
		return (-1);
	ft_putstr(curr);
	ft_putchar('\n');
	free(curr);
	return (1);
}
