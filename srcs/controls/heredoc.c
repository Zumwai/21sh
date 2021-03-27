/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 15:04:23 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 15:27:59 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static char		*trim_inword(char *line, int size, int count)
{
	char	*ret;
	int		i;
	int		j;

	ret = NULL;
	i = 0;
	j = 0;
	if (count)
	{
		ret = ft_strnew(size - count);
		while (line[i])
		{
			if (line[i] != '\'' && line[i] != '\"')
			{
				ret[j] = line[i];
				j++;
			}
			i++;
		}
		free(line);
		return (ret);
	}
	return (line);
}

char			*handle_eot(char *line, int size, int count, int i)
{
	char	*eot;

	eot = NULL;
	eot = ft_strndup(&line[i + 1], size);
	if (verify_char_heredoc(eot[size - 1]))
		eot[size - 1] = 0;
	if (count)
		eot = trim_inword(eot, size, count);
	return (eot);
}

static char		*grub_eot(char *line, int i, int *quotes)
{
	char	*eot;
	int		count;
	int		size;

	count = 0;
	size = 0;
	while (i > 0)
	{
		if (line[i] == '<' || line[i] == ' ')
			break ;
		if (line[i] == '\'' || line[i] == '\"')
		{
			(*quotes) = set_quotes(line[i], *quotes);
			count++;
		}
		i--;
		size++;
	}
	eot = handle_eot(line, size, count, i);
	return (eot);
}

extern int		update_hdoc_list(t_hdoc **lst, char *line)
{
	t_hdoc	*curs;
	int		size;
	int		ret;

	ret = 0;
	size = ft_strlen(line);
	if (!*lst)
		return (0);
	curs = *lst;
	while (curs)
	{
		if (!curs->eot)
		{
			while (line[curs->cord] == ' ')
				curs->cord--;
			curs->eot = grub_eot(line, curs->cord, &curs->quotes);
			ret = 1;
		}
		curs = curs->next;
	}
	return (ret);
}

extern t_hdoc	*clone_hdoc(t_hdoc *old)
{
	t_hdoc *new;
	t_hdoc *head;

	new = create_new_hdoc();
	head = new;
	new->next = NULL;
	while (old)
	{
		new->cord = old->cord;
		new->used = old->used;
		new->quotes = old->quotes;
		new->eot = ft_strdup(old->eot);
		new->next = NULL;
		old = old->next;
		if (old)
		{
			new->next = create_new_hdoc();
			new = new->next;
		}
	}
	new->used = false;
	return (head);
}
