/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 16:16:47 by aophion           #+#    #+#             */
/*   Updated: 2019/09/25 15:47:54 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmap(char const *s, char (*f)(char))
{
	size_t			i;
	size_t			len;
	char			*new;
	char			*temp;

	if (!(temp = (char *)s))
		return (NULL);
	i = 0;
	len = ft_strlen(temp);
	if (!(new = ft_strnew(len)))
		return (NULL);
	i = 0;
	while (temp[i] != '\0')
	{
		new[i] = f(temp[i]);
		i++;
	}
	return (new);
}
