/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/21 13:34:57 by aophion           #+#    #+#             */
/*   Updated: 2019/09/26 13:28:25 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"


static int		ft_check(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
			|| c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

int				ft_atoi(const char *str)
{
	size_t			i;
	long long		res;
	int				neg;

	neg = 0;
	i = 0;
	res = 0;
	while (ft_check(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			neg = 1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (res * 100 < res)
			return (neg ? 0 : -1);
		res = res * 10 + str[i] - '0';
		if (res < 0 && (neg) ? (-1) : (0))
			return (neg ? (-1) : (0));
		i++;
	}
	return (neg ? (int)-res : (int)res);
}
