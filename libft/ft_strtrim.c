/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 16:50:48 by aophion           #+#    #+#             */
/*   Updated: 2020/07/28 20:07:14 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s)
{
	size_t		len;
	size_t		i;

	i = 0;
	if (!s)
		return (NULL);
	len = ft_strlen(s);
	if (len == 0)
		return (NULL);
	while ((s[i] == ' ' || s[i] == '\n' || s[i] == '\t') && i < len)
		i++;
	while ((s[len - 1] == ' ' || s[len - 1] == '\n'
				|| s[len - 1] == '\t') && len > i)
		len--;
	if (i == len && len == 0)
	{
		return (ft_strdup(s));
	}
	else
	{
		return (ft_strsub(s, i, len - i));
	}
}
