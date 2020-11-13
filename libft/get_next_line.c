/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/20 13:19:20 by aophion           #+#    #+#             */
/*   Updated: 2019/11/28 14:18:25 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int		ft_new_node(t_glist **curs, int fd, char **mem)
{
	t_glist		*list;
	int			red;

	if ((red = read(fd, NULL, 0) < 0))
		return (-1);
	if (!(*mem = ft_strnew(BUFF_SIZE)))
		return (-1);
	if ((*curs)->fd != fd)
	{
		if (!(list = ft_memalloc(sizeof(t_glist))))
			return (-1);
		(*curs)->next = list;
		list->prev = (*curs);
		(*curs) = (*curs)->next;
		(*curs)->fd = fd;
		(*curs)->fin = 0;
		(*curs)->next = NULL;
	}
	return (1);
}

static int		ft_get_block(t_glist **curs, char **mem)
{
	char		*forcpy;
	ssize_t		red;

	if (!(forcpy = ft_strnew((*curs)->size + BUFF_SIZE)))
		return (-1);
	forcpy = ft_strcpy(forcpy, *mem);
	if (*mem || (*curs)->size >= BUFF_SIZE)
		free(*mem);
	*mem = forcpy;
	if ((red = read((*curs)->fd, &(*mem)[(*curs)->size], BUFF_SIZE))
			< BUFF_SIZE)
	{
		(*curs)->fin = 1;
		(*curs)->size += red;
	}
	else
		(*curs)->size += BUFF_SIZE;
	return (red);
}

static	int		ft_remain(t_glist **curs, char **ptr, char **line, char **mem)
{
	size_t		len;

	len = (*curs)->size - ft_strlen(*ptr);
	if (!(*line = ft_strnew(len)))
		return (-1);
	if (!(*line = ft_memcpy(*line, *mem, len)))
		return (-1);
	if (!((*curs)->temp = ft_strdup(*ptr + 1)))
		return (-1);
	(*curs)->size = (*curs)->size - len - 1;
	free(*mem);
	return (1);
}

static int		ft_check_forn(t_glist **curs, char **line, char **mem)
{
	char		*ptr;
	ssize_t		flag;

	flag = 1;
	while ((ptr = ft_strchr(*mem, '\n')) == NULL)
	{
		if (flag < 1 && (*curs)->fin == 1 && !*mem[0])
		{
			free(*mem);
			if ((*curs)->prev)
				(*curs)->prev->next = (*curs)->next;
			free(*curs);
			return (0);
		}
		if ((*curs)->fin == 1 && *mem[0] && flag == 0)
		{
			*line = *mem;
			(*curs)->fin = 2;
			return (1);
		}
		if ((flag = ft_get_block(curs, mem)) < 0)
			return (-1);
	}
	return (ft_remain(curs, &ptr, line, mem));
}

int				get_next_line(const int fd, char **line)
{
	static t_glist	start;
	int				flag;
	t_glist			*curs;
	char			*mem;

	start.fd = -200;
	if (fd < 0 || line == NULL)
		return (-1);
	curs = &start;
	while (curs->fd != fd && (curs->next))
		curs = curs->next;
	if (curs->fd == fd)
		mem = curs->temp;
	if (curs->fd != fd || curs->temp == NULL)
		if ((flag = ft_new_node(&curs, fd, &mem)) < 1)
			return (-1);
	if ((curs->fin == 1 && !curs->temp) || curs->fin == 2)
	{
		curs->prev->next = curs->next;
		if (curs->fin == 2 && !curs->temp)
			free(mem);
		free(curs);
		return (0);
	}
	return (ft_check_forn(&curs, line, &mem));
}
