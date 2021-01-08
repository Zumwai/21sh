/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/29 15:39:24 by student           #+#    #+#             */
/*   Updated: 2020/07/29 22:43:34 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	ft_concat(char *str, char **path, char *name)
{
	ft_strclr((*path));
	(*path) = strcpy((*path), str);
	(*path) = strcat((*path), "/");
	if (name)
		(*path) = strcat((*path), name);
}

char	*get_full_path(char *path, char *name)
{
	char	*str;
	char	buf[PATH_MAX];

	if (!(str = getcwd(buf, PATH_MAX)))
		return (NULL);
	path = ft_strnew(PATH_MAX);
	ft_strclr(path);
	path = ft_strcpy(path, str);
	path = ft_strcat(path, "/");
	path = ft_strcat(path, name);
	return (path);
}

int		putchar_like(int n)
{
	size_t	size;

	size = write(STDIN_FILENO, &n, 1);
	return (size);
}

int ft_abs(int i)
{
	return ((i > 0) ? i : -i);
}


char	*concat_lines(t_term *input)
{
	t_term	*curs;
	char	*line;
	char	*tmp;

	curs = input;
	line = NULL;
	while(curs)
	{
		tmp = line;
		if (!line && curs->new)
			line = ft_strdup(curs->new);
		else
			line = ft_strjoin(line, curs->new);
		free(tmp);
		curs = curs->next;
	}
	if (line && ft_strlen(line) > 4095)
	{
		handle_empty_error("usage: length", "input line too long, must be under 4096\n");
		set_free_null(&line);
		return (NULL);
	}
	return (line);
}