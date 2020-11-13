/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 17:36:58 by aophion           #+#    #+#             */
/*   Updated: 2019/09/25 19:35:04 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_numlen(int n)
{
	size_t		len;
	long long	num;

	num = (long long)n;
	len = 0;
	if (num < 0)
	{
		len++;
		num = -num;
	}
	while (num > 0)
	{
		num = num / 10;
		len++;
	}
	return (len);
}

char			*ft_itoa(int n)
{
	size_t		len;
	int			i;
	char		*a_num;
	long long	num;

	a_num = NULL;
	num = (long long)n;
	i = 0;
	len = ((n == 0) ? (1) : ft_numlen(n));
	if (!(a_num = ft_strnew(len)))
		return (NULL);
	if (n == 0)
		a_num[0] = '0';
	len--;
	if (num < 0)
	{
		a_num[0] = '-';
		num = -num;
	}
	while (len + ((n > 0) ? (1) : (0)) > 0)
	{
		a_num[len--] = num % 10 + '0';
		num = num / 10;
	}
	return (a_num);
}
