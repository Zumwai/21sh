/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 16:23:13 by aophion           #+#    #+#             */
/*   Updated: 2019/09/25 15:48:09 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t			i;
	size_t			len;
	char			*new;
	char			*temp;

	i = 0;
	if (!(temp = (char *)s))
		return (NULL);
	len = ft_strlen(temp);
	if (!(new = ft_strnew(len)))
		return (NULL);
	while (temp[i] != '\0')
	{
		new[i] = f(i, temp[i]);
		i++;
	}
	return (new);
}
