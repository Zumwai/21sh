/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 12:40:31 by aophion           #+#    #+#             */
/*   Updated: 2019/09/26 13:17:55 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t		i;
	size_t		j;
	char		*flag;
	size_t		check;

	i = 0;
	j = 0;
	flag = (char *)haystack;
	if (!(check = ft_strlen(needle)))
		return (flag);
	while (flag[i] != '\0' && i < len)
	{
		j = 0;
		while (flag[i + j] == needle[j]
				&& needle != 0 && i + j < len)
		{
			if (needle[j + 1] == '\0')
				return (&flag[i]);
			j++;
		}
		i++;
	}
	return (0);
}
