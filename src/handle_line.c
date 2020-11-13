/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/24 10:05:42 by student           #+#    #+#             */
/*   Updated: 2020/07/30 11:22:02 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		ft_comm(char **com, t_env **env)
{
	char	**seq;
	int		call;
	int		i;
	int		j;

	j = 0;
	i = 0;
	call = 1;
	while (com[i])
	{
		seq = ft_splitword(com[i]);
		if (seq)
		{
			call = go_do(seq, env);
			ft_free_tab(seq);
		}
		if (call == FIN)
			break ;
		i++;
	}
	return (call);
}

static int		ft_parse_inp(char *line, char **tmp)
{
	if (!line)
		return (0);
	(*tmp) = ft_strtrim(line);
	if ((*tmp) == NULL)
		return (0);
	if ((*tmp)[0] == '\0')
	{
		free((*tmp));
		return (0);
	}
	return (1);
}

int				ft_reader(t_env **ev)
{
	char	*line;
	char	*tmp;
	char	**com;
	int		i;

	line = NULL;
	tmp = NULL;
	i = get_next_line(0, &line);
	if (ft_parse_inp(line, &tmp))
	{
		com = ft_strsplit(tmp, ';');
		i = ft_comm(com, ev);
		ft_free_tab(com);
		if (tmp)
			free(tmp);
	}
	free(line);
	return (i);
}
