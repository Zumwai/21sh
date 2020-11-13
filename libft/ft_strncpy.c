/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 15:48:40 by aophion           #+#    #+#             */
/*   Updated: 2019/11/26 12:01:37 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncpy(char *dst, const char *src, size_t len)
{
	size_t		i;
	size_t		flag;

	flag = 0;
	i = 0;
	while (i < len)
	{
		if (src[i] == '\0' && flag == 0)
			flag = 1;
		if (flag == 1)
			dst[i] = '\0';
		else if (flag == 0)
			dst[i] = src[i];
		i++;
	}
	return (dst);
}
