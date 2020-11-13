/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 15:08:46 by aophion           #+#    #+#             */
/*   Updated: 2019/11/26 11:52:32 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	size_t			len;
	size_t			i;

	len = ft_strlen(s);
	if (s[0] == c && c == '\0')
		return ((char *)&s[0]);
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i + 1] == (char)c && s[i + 1] == '\0')
			return ((char*)&s[i + 1]);
		if (s[i] == (char)c)
			return ((char*)&s[i]);
		i++;
	}
	return (0);
}
