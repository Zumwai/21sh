/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/18 15:58:30 by aophion           #+#    #+#             */
/*   Updated: 2019/11/26 18:03:13 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcat(char *dest, const char *src)
{
	size_t			i;
	size_t			j;
	char			*temp;

	j = 0;
	i = 0;
	while (dest[i] != '\0')
		i++;
	temp = dest + i;
	while (src[j] != '\0')
	{
		temp[j] = src[j];
		j++;
	}
	temp[j] = '\0';
	return (dest);
}
