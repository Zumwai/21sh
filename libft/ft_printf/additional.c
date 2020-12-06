/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarsell <dmarsell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/05 11:49:51 by dmarsell          #+#    #+#             */
/*   Updated: 2020/08/06 11:42:12 by dmarsell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		to_weid_dec_min(char *ap, int precision, int *len, t_flags *f)
{
	if (f->space && (ap[0] != '-' || ap[0] != '0') && \
	f->type != 'u' && (f->weidth || f->precision) && \
	(f->weidth <= *len || f->precision >= *len) && !(f->plus))
	{
		write(FD, " ", 1);
		f->space = 0;
	}
	!(f->precision == 0 && f->dot) ? print_str_decimal(ap, len, f) : 1;
	f->space ? (*len)++ : 1;
	print_space_dec(ap, &precision, *len, f);
}

void		print_zero_decimal_next(char *ap, t_flags *f, int i)
{
	if (ap[0] == '-')
	{
		f->print ? write(FD, "-", 1) : 1;
		f->plus = 0;
		ap[0] = '0';
		!(f->precision) ? i-- : 1;
	}
	while (i-- > 0)
		f->print ? write(FD, "0", 1) : 1;
}

void		print_zero_decimal(char *ap, int *len, t_flags *f)
{
	int		i;

	i = f->weidth - *len;
	if (f->space && (ap[0] != '-' || ap[0] != '0') \
	&& (f->type != 'u' || f->type != 'U') \
	&& (f->weidth < *len || f->precision >= *len) && !(f->plus))
	{
		write(FD, " ", 1);
		f->space = 0;
	}
	if (f->plus && ap[0] != '-')
	{
		f->print ? write(FD, "+", 1) : 1;
		f->plus = 0;
		(*len)++;
		f->precision ? i++ : 1;
		i--;
	}
	print_zero_decimal_next(ap, f, i);
}

void		print_hex(long nb, int *pointlen, t_flags *f)
{
	if (nb >= 16)
	{
		print_hex(nb / 16, pointlen, f);
		print_hex(nb % 16, pointlen, f);
	}
	if (nb < 10)
	{
		if (f->print)
			ft_putchar_fd(nb + 48, FD);
		(*pointlen)++;
	}
	else if (nb < 16)
	{
		if (f->print)
			ft_putchar_fd(nb + 87, FD);
		(*pointlen)++;
	}
}

void		only_zero(char *s)
{
	int		i;

	i = 0;
	while (i < 3)
	{
		s[i] = '\0';
		i++;
	}
}
