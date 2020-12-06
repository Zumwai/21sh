/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_decimal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarsell <dmarsell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/13 11:03:56 by dmarsell          #+#    #+#             */
/*   Updated: 2020/08/06 11:42:47 by dmarsell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		print_str_decimal_next(char *ap, int *len, t_flags *f, int i)
{
	if (!(f->precision) && f->plus && ap[i] != '-' && ap[0] != '0')
	{
		if (f->print && f->type != 'u')
		{
			write(FD, "+", 1);
			(*len)++;
		}
		f->plus = 0;
		f->space ? (*len)-- : 1;
	}
	while (i < *len && ap[i])
	{
		f->print ? write(FD, &ap[i], 1) : 1;
		i++;
	}
}

void		print_str_decimal(char *ap, int *len, t_flags *f)
{
	int		i;

	i = 0;
	if (f->space && (ap[0] != '-' || ap[0] != '0') && \
	!(f->plus) && f->type != 'u')
	{
		write(FD, " ", 1);
		f->space = 0;
		f->percent = -1;
		(*len)++;
	}
	if ((f->precision && f->plus && ap[i] != '-' && ap[0] != '0')\
	|| (ap[0] == '0' && f->plus))
	{
		f->print ? write(FD, "+", 1) : 1;
		f->plus = 0;
		(*len)++;
	}
	print_str_decimal_next(ap, len, f, i);
}

int			print_char_decimal_next_n(t_flags *f, char *ap, t_print_d *data)
{
	if (f->minus)
	{
		data->weidth = data->weidth - (data->precision - data->len);
		f->plus && ap[0] != '-' ? data->precision++ : 1;
		ap[0] == '-' ? data->precision++ : 1;
		f->space ? data->precision++ : 1;
		data->len <= f->precision ? \
		print_zero_decimal(ap, &data->weidth, f) : 1;
		print_str_decimal(ap, &data->len, f);
		print_space_dec(ap, &data->precision, data->len, f);
		return (f->weidth - 1);
	}
	f->plus && ap[0] != '-' ? data->precision-- : 1;
	!(f->zero && ap[0] == '-' && (data->weidth - 1) == data->len) ? \
	print_space_dec(ap, &data->precision, data->len, f) : 1;
	f->plus && ap[0] != '-' ? data->precision++ : 1;
	if (data->precision < data->len)
		data->precision = data->weidth - data->precision;
	else
	{
		data->precision = data->weidth - (data->precision - data->len);
		print_zero_decimal(ap, &data->precision, f);
	}
	print_str_decimal(ap, &data->len, f);
	return (f->weidth - 1);
}

int			print_char_decimal_next(t_flags *f, char *ap, t_print_d *data)
{
	if (data->len >= data->precision && data->len >= data->weidth)
	{
		print_str_decimal(ap, &data->len, f);
		return (data->len - 1);
	}
	else if (data->weidth > data->precision)
		return (print_char_decimal_next_n(f, ap, data));
	else if (data->weidth < data->precision)
	{
		f->plus && ap[0] != '-' ? f->precision++ : 1;
		data->precision = data->weidth - (data->precision - data->len);
		print_zero_decimal(ap, &data->precision, f);
		print_str_decimal(ap, &data->len, f);
		if (ap[0] == '0' || ap[0] == '-')
			return (f->precision);
		return (f->precision - 1);
	}
	return (0);
}

int			print_char_decimal(t_flags *f, char *ap)
{
	t_print_d		data;

	data.weidth = f->weidth;
	data.precision = f->precision;
	data.len = ft_strlen(ap);
	f->plus || ap[0] == '-' ? f->space = 0 : 1;
	if (f->precision)
		return (print_char_decimal_next(f, ap, &data));
	else
	{
		data.len = to_weidth_dec(ap, data.precision, &data.len, f);
		return (data.len - 1);
	}
	return (0);
}
