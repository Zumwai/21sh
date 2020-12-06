/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing_nums.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarsell <dmarsell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/06 16:12:39 by dmarsell          #+#    #+#             */
/*   Updated: 2020/08/06 11:50:39 by dmarsell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	only_weidth_next_n_n(char *s, int l, t_flags *f, t_count_d *rx)
{
	if (f->zero == 1)
	{
		if (f->hash && f->type != 'o')
		{
			f->print ? ft_putstr(f->ox) : 1;
			rx->space += 2;
			put_zero(l + rx->space, f->weidth, f);
		}
		else
			put_zero(l, f->weidth, f);
		f->print ? ft_putstr(s) : 1;
	}
	if ((f->weidth >= (l + rx->space) && f->div == 16) || f->type == 'o')
		rx->start = f->weidth;
	else
		rx->start = l + rx->space;
}

void	only_weidth_next_n(char *s, int l, t_flags *f, t_count_d *rx)
{
	if (f->zero == 0)
	{
		put_space(l, f);
		if (f->hash && f->type != 'o')
		{
			f->print ? ft_putstr(f->ox) : 1;
			rx->space += 2;
		}
		f->print ? ft_putstr(s) : 1;
	}
	only_weidth_next_n_n(s, l, f, rx);
}

void	only_weidth_next(char *s, int l, t_flags *f, t_count_d *rx)
{
	if (f->hash && f->type != 'o')
	{
		f->print ? ft_putstr(f->ox) : 1;
		rx->space += 2;
	}
	f->print ? ft_putstr(s) : 1;
	put_space(l, f);
	if ((f->weidth >= (l + rx->space) && f->div == 16) || f->type == 'o')
		rx->start = f->weidth;
	else
		rx->start = l + rx->space;
}

int		only_weidth(char *s, int l, t_flags *f)
{
	t_count_d	rx;

	rx.space = 0;
	rx.start = 0;
	if (f->weidth == 0 || f->weidth < l)
	{
		if (f->hash && f->type != 'o')
		{
			f->print ? ft_putstr(f->ox) : 1;
			rx.space += 2;
		}
		f->print ? ft_putstr(s) : 1;
		rx.start = l + rx.space;
	}
	if (f->weidth >= l)
	{
		f->minus == 1 ? only_weidth_next(s, l, f, &rx) : 1;
		f->minus == 0 ? only_weidth_next_n(s, l, f, &rx) : 1;
	}
	return (rx.start);
}

int		print_num_str(char *s, t_flags *f)
{
	int		l;
	int		res;

	if (f->type == 'x')
		f->ox = "0x";
	else
		f->ox = "0X";
	res = 0;
	l = ft_strlen(s);
	l >= f->precision && f->precision > 0 ? res = l_big(s, f, l) : 1;
	l < f->precision ? res = l_less(s, f, l) : 1;
	f->precision == 0 ? res = only_weidth(s, l, f) : 1;
	return (res);
}
