/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_addres.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarsell <dmarsell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 15:02:03 by dmarsell          #+#    #+#             */
/*   Updated: 2020/08/06 10:47:34 by dmarsell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		print_for_point(int pointlen, char sym, t_flags *f, int b)
{
	int		i;
	int		res;

	if (b)
		i = f->precision - pointlen;
	else
		i = f->weidth - pointlen;
	res = i;
	while (i > 0)
	{
		if (f->print)
			ft_putchar_fd(sym, FD);
		i--;
	}
	return (res);
}

void	find_point_len(long nb, int *pointlen)
{
	if (nb >= 16)
	{
		find_point_len(nb / 16, pointlen);
		find_point_len(nb % 16, pointlen);
	}
	if (nb < 10)
		(*pointlen)++;
	else if (nb < 16)
		(*pointlen)++;
}

int		print_address_next_n(t_flags *f, int pointlen, long p, int bit)
{
	if (f->weidth > pointlen && !(f->minus))
	{
		pointlen += 2;
		pointlen = print_for_point(pointlen, ' ', f, bit);
		f->print ? ft_putstr("0x") : 1;
		print_hex(p, &pointlen, f);
		return (f->weidth);
	}
	else if (f->precision >= f->weidth && f->precision > (pointlen - 2))
	{
		f->print ? ft_putstr("0x") : 1;
		bit = 1;
		pointlen = print_for_point(pointlen, '0', f, bit);
		print_hex(p, &pointlen, f);
		pointlen += 2;
		return (pointlen);
	}
	pointlen = 2;
	f->print ? ft_putstr("0x") : 1;
	print_hex(p, &pointlen, f);
	return (pointlen);
}

int		print_address_next(t_flags *f, int bit)
{
	if (f->minus && f->weidth)
	{
		f->print ? ft_putstr("0x0") : 1;
		f->weidth > 3 ? print_space(3, f) : 1;
		return (f->weidth);
	}
	else if (f->weidth)
	{
		f->weidth > 3 ? print_space(3, f) : 1;
		f->print ? ft_putstr("0x0") : 1;
		return (f->weidth);
	}
	if (f->precision)
	{
		ft_putstr("0x");
		bit = print_for_point(0, '0', f, 1);
		return (bit + 2);
	}
	if (!(f->precision) && f->dot)
	{
		ft_putstr("0x");
		return (2);
	}
	f->print ? ft_putstr("0x0") : 1;
	return (3);
}

int		print_address(t_flags *f, va_list ap)
{
	int		pointlen;
	int		bit;
	long	p;

	p = va_arg(ap, long int);
	bit = 0;
	pointlen = 0;
	find_point_len(p, &pointlen);
	if (p == 0)
		return (print_address_next(f, bit));
	if (f->minus)
	{
		pointlen = 2;
		f->print ? ft_putstr("0x") : 1;
		print_hex(p, &pointlen, f);
		pointlen = print_for_point(pointlen, ' ', f, bit);
	}
	else
		return (print_address_next_n(f, pointlen, p, bit));
	return (pointlen);
}
