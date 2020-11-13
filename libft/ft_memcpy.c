/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 13:18:28 by aophion           #+#    #+#             */
/*   Updated: 2019/11/26 15:26:00 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	char			*ch_dest;
	const char		*ch_src;

	if (!dst && !src && n > 0)
		return (0);
	ch_dest = (char *)dst;
	ch_src = (char *)src;
	i = 0;
	while (i < n)
	{
		ch_dest[i] = ch_src[i];
		i++;
	}
	return (dst);
}
