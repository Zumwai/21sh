/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_ifdir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 10:13:39 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 11:02:00 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void	insert_parts(char *path, char *new, char *orig, char *pwd)
{
	if (orig[0] != '/')
	{
		ft_strcat(path, pwd);
		ft_strcat(path, "/");
	}
	ft_strcat(path, orig);
	if (new && new[0])
	{
		ft_strcat(path, "/");
		ft_strcat(path, new);
	}
}

static char	*max_path(char *orig, char *new)
{
	char	*pwd;
	char	*path;
	int		size;

	pwd = NULL;
	if (!(pwd = getcwd(pwd, PATH_MAX)))
		handle_exit_errors("GETCWD got PWND");
	size = ft_strlen(orig) + ft_strlen(new) + 1;
	if (orig[0] != '/')
		size += ft_strlen(pwd);
	if (size < PATH_MAX)
		size = PATH_MAX;
	path = ft_strnew(size);
	insert_parts(path, new, orig, pwd);
	free(pwd);
	return (path);
}

int			check_for_dir(char *orig, char *new)
{
	unsigned int	tmp;
	struct stat		per;
	char			*path;
	int				ret;
	int				len;

	ft_memset(&per, 0, sizeof(struct stat));
	path = max_path(orig, new);
	ret = 0;
	if ((lstat(path, &per)) == -1)
		ret = 0;
	else
	{
		tmp = (per.st_mode & S_IFMT);
		if (S_ISDIR(tmp))
		{
			len = ft_strlen(path);
			ret = 1;
			if (path[len - 1] == '/')
				ret = 0;
		}
	}
	free(path);
	return (ret);
}
