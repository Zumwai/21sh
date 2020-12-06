/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_float_next.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarsell <dmarsell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/07 21:55:35 by dmarsell          #+#    #+#             */
/*   Updated: 2020/08/06 11:42:36 by dmarsell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_str_connect(char *entire, char *bufer, char *sign, int p)
{
	int		len;
	char	*res;

	len = ft_strlen(sign) + ft_strlen(entire) + p;
	len = p > 0 ? len + 1 : len;
	if (!(res = ft_strnew(len)))
		return (0);
	ft_strcat(res, sign);
	ft_strcat(res, entire);
	if (p > 0)
	{
		ft_strcat(res, ".");
		ft_strcat(res, bufer);
		ft_memset(res + len - (p - ft_strlen(bufer)),\
		'0', p - ft_strlen(bufer));
	}
	return (res);
}

void	ft_to_str(char (*bufer)[70], int n)
{
	int i;

	n > 70 ? n = 70 : n;
	i = 0;
	while (i < n)
	{
		(*bufer)[i] = (*bufer)[i] + '0';
		i++;
	}
}

int		ft_circ(char (*bufer)[70], int p)
{
	int num;

	num = (*bufer)[p] < 5 ? 0 : 1;
	(*bufer)[p] = 0;
	while (num == 1 && p > 0)
	{
		num = ((*bufer)[p - 1] + 1) / 10;
		(*bufer)[p - 1] = ((*bufer)[p - 1] + 1) % 10;
		p--;
	}
	return (num);
}

int		ft_div_base(unsigned long long value, int base)
{
	int num;

	num = 0;
	while (value > 0)
	{
		value = value / base;
		num++;
	}
	if (num == 0)
		return (1);
	return (num);
}
