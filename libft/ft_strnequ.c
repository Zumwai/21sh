/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnequ.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 16:29:12 by aophion           #+#    #+#             */
/*   Updated: 2019/09/25 15:48:20 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strnequ(char const *s1, char const *s2, size_t n)
{
	size_t			i;
	unsigned char	*c_s1;
	unsigned char	*c_s2;

	if (!s1 || !s2)
		return (0);
	c_s1 = (unsigned char*)s1;
	c_s2 = (unsigned char*)s2;
	i = 0;
	while ((c_s1[i] != '\0' || c_s2[i] != '\0') && i < n)
	{
		if (c_s1[i] != c_s2[i])
			return (0);
		i++;
	}
	return (1);
}
