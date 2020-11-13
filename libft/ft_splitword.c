/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_splitword.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/28 19:55:42 by student           #+#    #+#             */
/*   Updated: 2020/07/29 14:38:40 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_letter_check(char c)
{
	if (c == ' ' || c == '\t' || c == '\r'
			|| c == '\n' || c == '\v' || c == '\f')
		return (0);
	return (1);
}

static int	ft_count_word(char *line)
{
	int		ret;
	int		flag;
	int		prev;
	int		i;

	i = 0;
	ret = 0;
	prev = 0;
	while (line[i])
	{
		flag = 0;
		if (ft_letter_check(line[i]))
			flag = 1;
		if (prev == 0 && flag == 1)
		{
			prev = 1;
			ret++;
		}
		else if (prev == 1 && flag == 0)
			prev = 0;
		i++;
	}
	return (ret);
}

static void	*ft_free_loc(char **tab)
{
	int		i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

static int	ft_len(char *l, int *s)
{
	int		ret;
	int		len;

	len = ft_strlen(l);
	ret = 0;
	while (len > (*s) && !ft_letter_check(l[(*s)]))
		(*s)++;
	while (len > (*s) + ret && ft_letter_check(l[(*s) + ret]))
		ret++;
	return (ret);
}

char		**ft_splitword(char *l)
{
	char	**tab;
	int		size;
	int		i;
	int		ret;
	int		s;

	if (!l)
		return (NULL);
	size = ft_count_word(l);
	if (!size)
		return (NULL);
	tab = ft_newdim(size + 1);
	s = 0;
	i = 0;
	while (i < size)
	{
		ret = ft_len(l, &s);
		tab[i] = ft_strnew(ret + 1);
		tab[i] = ft_strncpy(tab[i], &l[s], ret);
		i++;
		s += ret;
	}
	return (tab);
}
