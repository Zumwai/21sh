/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing_nums_next.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarsell <dmarsell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/06 17:29:46 by dmarsell          #+#    #+#             */
/*   Updated: 2020/08/06 10:51:13 by dmarsell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		l_big_next(char *s, t_flags *f, int l, int x)
{
	if (f->minus == 0)
	{
		put_space(l, f);
		if (f->hash && f->type != 'o')
		{
			f->print ? ft_putstr(f->ox) : 1;
			x += 2;
		}
		f->print ? ft_putstr(s) : 1;
	}
	if (f->minus == 1)
	{
		if (f->hash && f->type != 'o')
		{
			f->print ? ft_putstr(f->ox) : 1;
			x += 2;
		}
		f->print ? ft_putstr(s) : 1;
		put_space(l, f);
	}
	if ((f->weidth >= (l + x) && f->div == 16) || f->type == 'o')
		return (f->weidth);
	else
		return (l + x);
}

int		l_big(char *s, t_flags *f, int l)
{
	int		x;

	x = 0;
	if (l >= f->weidth)
	{
		if (f->hash && f->type != 'o')
		{
			f->print ? ft_putstr(f->ox) : 1;
			x += 2;
		}
		f->print ? ft_putstr(s) : 1;
		return (l + x);
	}
	if (l < f->weidth)
		return (l_big_next(s, f, l, x));
	return (0);
}

void	l_less_next_n(char *s, t_flags *f, int l, t_count_d *rx)
{
	if (f->minus == '\0')
	{
		put_space(l, f);
		if (f->hash && f->type != 'o')
		{
			f->print ? ft_putstr(f->ox) : 1;
			rx->start += 2;
		}
		put_zero(l, f->precision, f);
		f->print ? ft_putstr(s) : 1;
	}
	if (f->minus == 1)
	{
		if (f->hash && f->type != 'o')
		{
			f->print ? ft_putstr(f->ox) : 1;
			rx->start += 2;
		}
		put_zero(l, f->precision, f);
		f->print ? ft_putstr(s) : 1;
		put_space(l, f);
	}
}

void	l_less_next(char *s, t_flags *f, int l, t_count_d *rx)
{
	if (f->hash && f->type != 'o')
	{
		f->print ? ft_putstr(f->ox) : 1;
		rx->space += 2;
	}
	put_zero(l, f->precision, f);
	f->print ? ft_putstr(s) : 1;
	if (f->precision >= (l + rx->space) && f->type != 'o')
		rx->start = f->precision + rx->space;
	else if (f->type != 'o')
		rx->start = l + rx->space;
	else
		rx->start = f->precision;
}

int		l_less(char *s, t_flags *f, int l)
{
	t_count_d	rx;

	rx.start = 0;
	rx.space = 0;
	f->precision > f->weidth ? l_less_next(s, f, l, &rx) : 1;
	if (f->precision < f->weidth)
	{
		l_less_next_n(s, f, l, &rx);
		if ((f->weidth >= (l + rx.space) && f->div == 16) || f->type == 'o')
			rx.start = f->weidth;
		else
			rx.start = l + rx.space;
	}
	return (rx.start);
}
