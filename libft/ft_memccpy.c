/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 13:34:50 by aophion           #+#    #+#             */
/*   Updated: 2019/09/26 13:27:51 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst,
		const void *src, int c, size_t n)
{
	size_t					i;
	unsigned char			*ch_dest;
	const unsigned char		*ch_src;
	unsigned char			ch_c;
	char					*check;

	ch_dest = (unsigned char *)dst;
	ch_src = (unsigned const char *)src;
	ch_c = (unsigned char)c;
	i = 0;
	check = 0;
	while (i < n && check == 0)
	{
		if (ch_src[i] == ch_c)
			check = dst + i + 1;
		ch_dest[i] = ch_src[i];
		i++;
	}
	return (check);
}
