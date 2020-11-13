/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 16:43:48 by aophion           #+#    #+#             */
/*   Updated: 2019/09/24 16:36:09 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	d_len;
	size_t	j;
	size_t	f_len;

	d_len = ft_strlen(dest);
	f_len = 0;
	f_len = ft_strlen(src);
	if (size <= d_len)
		f_len = f_len + size;
	else
		f_len = f_len + d_len;
	j = 0;
	while (src[j] != '\0' && d_len + 1 < size)
	{
		dest[d_len] = src[j];
		d_len++;
		j++;
	}
	dest[d_len] = '\0';
	return (f_len);
}
