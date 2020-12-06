/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   begin.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarsell <dmarsell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 13:31:35 by cassunta          #+#    #+#             */
/*   Updated: 2020/08/06 10:40:32 by dmarsell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		if_percent(const char *ft, int i)
{
	while ((ft[i] != 'd') && (ft[i] != 'i') && (ft[i] != 'o')
	&& (ft[i] != 'u') && (ft[i] != 'x') && (ft[i] != 'X')
	&& (ft[i] != 'c') && (ft[i] != 's') && (ft[i] != 'p')
	&& (ft[i] != 'f') && (ft[i] != 'D') && (ft[i] != 'I')
	&& (ft[i] != 'O') && (ft[i] != 'U') && (ft[i] != 'S')
	&& (ft[i] != 'P') && (ft[i] != 'F') && (ft[i] != 'C')
	&& (ft[i] != 'b') && (ft[i] != '\0'))
	{
		i++;
	}
	return (i);
}

void	put_zero(int s, int p, t_flags *f)
{
	int		delta;

	delta = p - s;
	while (delta--)
		f->print ? ft_putchar_fd('0', FD) : 1;
}

void	ecran(int *res, int *i)
{
	*i += 2;
	*res += 1;
	write(FD, "%", 1);
}

void	ordinary(const char ft, int *i, int *res)
{
	write(FD, &ft, 1);
	(*res)++;
	(*i)++;
}

int		ft_printf(const char *ft, ...)
{
	va_list	ap;
	int		res;
	int		len;
	int		b;
	int		i;

	i = 0;
	res = 0;
	len = ft_strlen(ft);
	va_start(ap, ft);
	while (i < len)
	{
		ft[i] != '%' ? ordinary(ft[i], &i, &res) : 1;
		ft[i] == '%' && ft[i + 1] == '%' ? ecran(&res, &i) : 1;
		if (ft[i] == '%' && ft[i + 1] != '%')
		{
			b = i;
			res = res + (int)all_formats(ft, i, ap);
			i = if_percent(ft, b) + 1;
		}
	}
	if (ft[i - 1] == '\0' && ft[i - 3] == '%' && len > 1)
		ordinary(ft[i - 2], &i, &res);
	va_end(ap);
	return (res);
}
