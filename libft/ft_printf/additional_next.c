/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   additional_next.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarsell <dmarsell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/06 15:04:53 by dmarsell          #+#    #+#             */
/*   Updated: 2020/08/06 10:40:10 by dmarsell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	print_space_float(t_use *box)
{
	write(FD, &box->space, 1);
	box->plus = 0;
	box->space = 0;
}

void	ft_decminus(t_use *box)
{
	write(FD, "-", 1);
	box->decminus = '2';
}

int		how_much(unsigned int tmp, t_flags *f)
{
	int	c;

	c = 1;
	while (tmp / f->div != 0)
	{
		c++;
		tmp = tmp / f->div;
	}
	return (c);
}

int		how_much_l(unsigned long tmp, t_flags *f)
{
	int		c;

	c = 0;
	while (tmp / f->div != 0)
	{
		c++;
		tmp = tmp / f->div;
	}
	return (c);
}

void	put_space(int l, t_flags *f)
{
	int		i;
	int		c;

	i = (l > f->precision ? l : f->precision);
	c = f->weidth - i;
	if (f->hash && f->type != 'o')
		c -= 2;
	while (c)
	{
		if (f->hash && f->type != 'o' && c < 0)
			break ;
		f->print ? write(FD, " ", 1) : 1;
		c--;
	}
}
