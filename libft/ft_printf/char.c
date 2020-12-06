/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarsell <dmarsell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/05 11:55:36 by dmarsell          #+#    #+#             */
/*   Updated: 2020/08/06 10:42:11 by dmarsell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int			print_ch_null_next_n(t_flags *f, int res, char *nu)
{
	if (f->minus)
	{
		ft_putstr(nu);
		print_space(res, f);
	}
	else
	{
		print_space(res, f);
		ft_putstr(nu);
	}
	if (f->weidth > res)
		return (f->weidth);
	return (res);
}

int			print_ch_null_next(t_flags *f, int res, char *nu)
{
	if ((f->precision < f->weidth)
	&& f->precision < res && f->precision)
	{
		if (f->minus)
		{
			ft_putnstr(nu, f->precision, res);
			print_space(f->precision, f);
		}
		else
		{
			print_space(f->precision, f);
			ft_putnstr(nu, f->precision, res);
		}
		return (f->weidth);
	}
	return (print_ch_null_next_n(f, res, nu));
}

int			print_ch_null(t_flags *f)
{
	int		res;
	char	*nu;

	nu = "(null)";
	res = 6;
	if (f->weidth)
	{
		if (!(f->precision) && f->dot)
		{
			print_space(0, f);
			return (f->weidth);
		}
		return (print_ch_null_next(f, res, nu));
	}
	if (f->precision < res && f->precision != 0)
	{
		ft_putnstr(nu, f->precision, res);
		return (f->precision);
	}
	else
		ft_putstr(nu);
	return (res);
}

int			print_ch_next(t_flags *f, char *v, int len)
{
	int		res;

	res = 0;
	if (!(f->precision) && f->weidth && f->dot)
	{
		if (f->print)
			print_space(0, f);
		return (f->weidth);
	}
	if (f->precision == 0 || f->precision == len || f->precision > len)
	{
		res = to_weidth(v, len, f);
		return (res);
	}
	if (f->precision < len)
	{
		res = to_weidth(v, f->precision, f);
		return (res);
	}
	return (0);
}

int			print_ch(t_flags *f, va_list ap)
{
	int		len;
	char	*v;
	char	c;

	if (f->type == 'c' || f->type == 'C')
	{
		len = 1;
		c = va_arg(ap, int);
		c &= 0xFF;
		v = &c;
	}
	else
	{
		v = va_arg(ap, char *);
		if ((f->dot && !(f->star) && (f->precision == 48 && f->weidth == 48))
		|| (f->dot && !(f->star) && (f->weidth == 0 && f->precision == 0))
		|| (f->dot && !(f->star) && (f->precision == 48 && f->weidth == 0))
		|| (f->dot && !(f->star) && (f->precision == 0 && f->weidth == 48)))
			return (0);
		if (v == NULL || f->star)
			return (print_ch_null(f));
		len = ft_strlen(v);
	}
	return (print_ch_next(f, v, len));
}
