/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 17:15:29 by aophion           #+#    #+#             */
/*   Updated: 2019/09/25 17:44:18 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	word_count(const char *str, char c)
{
	size_t		count;
	size_t		flag;
	size_t		i;

	i = 0;
	count = 0;
	flag = 0;
	while (str[i] != '\0')
	{
		if (str[i] != c && flag == 0)
		{
			flag = 1;
			count++;
		}
		else if (str[i] != c && flag == 1)
			i++;
		else if (str[i] == c && flag == 1)
			flag = 0;
		else
			i++;
	}
	return (count);
}

static size_t	ft_len(const char *s, char c)
{
	const char *str;

	str = s;
	if (*str && c)
	{
		while (*s != c && *s)
			s++;
		return (s - str);
	}
	return (0);
}

static void		ft_free(char **tab)
{
	size_t		i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
}

char			**ft_strsplit(char const *s, char c)
{
	size_t			j;
	char			**tab;
	size_t			size;

	if (!s)
		return (NULL);
	size = word_count(s, c);
	if (!(tab = (char **)malloc(sizeof(char *) * (size + 1))))
		return (NULL);
	j = 0;
	while (size)
	{
		while (*s == c && *s != '\0')
			s++;
		if (!(tab[j] = ft_strncpy(ft_strnew(ft_len(s, c)), s, ft_len(s, c))))
		{
			ft_free(tab);
			return (NULL);
		}
		j++;
		s = s + ft_len(s, c);
		size--;
	}
	tab[j] = NULL;
	return (tab);
}
