/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 12:33:55 by aophion           #+#    #+#             */
/*   Updated: 2019/09/25 14:56:17 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	len;

	if (c == '\0')
		return ((char *)s + ft_strlen(s));
	len = ft_strlen(s);
	while (len-- > 0)
	{
		if (s[len] == c)
			return ((char *)&s[len]);
	}
	return (0);
}
