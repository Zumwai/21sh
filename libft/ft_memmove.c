/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 13:59:41 by aophion           #+#    #+#             */
/*   Updated: 2019/09/24 16:08:52 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t size)
{
	unsigned char			*c_dest;
	const unsigned char		*c_src;
	size_t					i;

	i = 0;
	c_dest = (unsigned char *)dst;
	c_src = (const unsigned char *)src;
	if (!src && !dst && size > 0)
		return (NULL);
	if (c_src < c_dest)
	{
		while (i++ < size)
			c_dest[size - i] = c_src[size - i];
	}
	else
	{
		while (i < size)
		{
			c_dest[i] = c_src[i];
			i++;
		}
	}
	return (dst);
}
