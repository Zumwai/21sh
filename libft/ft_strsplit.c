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
/*
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
*/
static int		ft_word_len(char const *s, char c)
{
	int let;

	let = 0;
	while (*s && *s != c)
	{
		let++;
		s++;
	}
	return (let);
}

static int		ft_count_words(char const *s, char c)
{
	int i;
	int word;

	i = 0;
	word = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			word++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (word);
}

static void		ft_free(char **s, int i)
{
	while (i--)
		ft_strdel(&(s[i]));
	free(*s);
}

static char		**ft_create_word(const char *s, char c, int nb, char **arr)
{
	int			i;
	int			j;

	i = 0;
	j = 0;
	while (i < nb)
	{
		if (s[j] && s[j] != c)
		{
			arr[i] = ft_strsub(s, j, ft_word_len(&s[j], c));
			if (arr[i])
			{
				i++;
				j = j + (ft_word_len(&s[j], c));
			}
			else
			{
				ft_free(arr, i);
				return (NULL);
			}
		}
		j++;
	}
	arr[nb] = NULL;
	return ((char **)arr);
}

char			**ft_strsplit(char const *s, char c)
{
	char		**arr;
	int			word_nb;
	int			i;
	int			j;

	j = 0;
	i = 0;
	if (s == NULL)
		return (NULL);
	word_nb = ft_count_words(s, c);
	arr = (char **)malloc(sizeof(char *) * (word_nb + 1));
	if (arr == NULL)
		return (NULL);
	ft_create_word(s, c, word_nb, arr);
	return (arr);
}