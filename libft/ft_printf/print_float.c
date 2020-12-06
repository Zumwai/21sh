/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_float.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarsell <dmarsell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/07 22:16:56 by dmarsell          #+#    #+#             */
/*   Updated: 2020/08/06 10:49:30 by dmarsell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		print_plus_float(t_use *box, t_flags *f)
{
	if (f->plus && !(box->plus) && !(box->decminus))
	{
		write(FD, "+", 1);
		box->res++;
	}
	else
	{
		write(FD, &box->plus, 1);
		box->plus = 0;
		box->space = 0;
	}
}

void		print_sym_float(t_use *box, t_flags *f)
{
	int		i;
	char	sym;

	sym = 0;
	i = 0;
	i = f->weidth - box->res;
	sym = f->zero ? '0' : ' ';
	while (i-- > 0)
	{
		write(FD, &sym, 1);
		box->res++;
	}
}

void		print_str_float(char *str, t_use *box, int len)
{
	int		i;

	i = 0;
	box->decminus == '2' ? i++ : 1;
	while (i < len && str[i])
		write(FD, &str[i++], 1);
	box->decminus == '2' ? box->decminus = 0 : 1;
}

void		print_float_next(t_flags *f, char *buff, t_use *box)
{
	if (f->minus)
	{
		f->plus && !(box->decminus) ? print_plus_float(box, f) : 1;
		print_str_float(buff, box, box->res);
		f->zero = 0;
		print_sym_float(box, f);
	}
	else if (!(f->minus))
	{
		box->plus && f->zero ? print_plus_float(box, f) : 1;
		box->decminus == '-' && f->zero ? ft_decminus(box) : 1;
		print_sym_float(box, f);
		box->plus ? print_plus_float(box, f) : 1;
		f->zero && box->decminus == '2' ? \
		print_str_float(buff, box, box->len) : ft_putstr(buff);
	}
}

int			print_float(t_flags *f, char *buff)
{
	t_use		box;

	box.plus = f->plus ? '+' : '\0';
	box.space = f->space ? ' ' : '\0';
	box.decminus = f->decminus ? '-' : '\0';
	if (box.decminus)
	{
		box.plus = 0;
		box.space = 0;
	}
	box.len = ft_strlen(buff);
	box.res = box.len;
	(box.space || box.plus) && !(box.decminus) ? box.res++ : 1;
	box.plus && !(f->weidth > box.res) ? print_plus_float(&box, f) : 1;
	box.space ? print_space_float(&box) : 1;
	if (f->weidth > box.res)
		print_float_next(f, buff, &box);
	else
	{
		box.decminus == '-' ? ft_decminus(&box) : 1;
		box.decminus == '2' ? print_str_float(buff, &box, box.len) :\
		ft_putstr(buff);
	}
	return (box.res);
}
