/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 14:30:46 by aophion           #+#    #+#             */
/*   Updated: 2019/09/26 13:26:43 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	char			*spag;
	size_t			i;
	unsigned char	check;
	char			*sup;

	spag = 0;
	i = 0;
	sup = (char *)s;
	check = (unsigned char)c;
	if (n)
	{
		while (i < n)
		{
			if ((unsigned char)sup[i] == check)
				return (spag = sup + i);
			i++;
		}
	}
	return (spag);
}
