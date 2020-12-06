/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_next.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarsell <dmarsell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/05 12:27:37 by dmarsell          #+#    #+#             */
/*   Updated: 2020/08/06 10:42:37 by dmarsell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int			to_weidth(char *ap, int c, t_flags *f)
{
	if (c >= f->weidth)
	{
		if (f->print)
			print_str(ap, c);
		return (c);
	}
	if (c < f->weidth)
	{
		if (f->minus == 0 && f->print)
		{
			if (f->zero && f->print)
				print_zero(c, f);
			else
				print_space(c, f);
			print_str(ap, c);
		}
		if (f->minus == 1 && f->print)
		{
			print_str(ap, c);
			print_space(c, f);
		}
	}
	return (f->weidth);
}

void		ft_putnstr(char *str, int n, int end)
{
	int		i;

	i = 0;
	while (i < n && i < end)
		ft_putchar_fd(str[i++], FD);
}

void		print_str(char *ap, int l)
{
	int		i;

	i = 0;
	while (l > 0)
	{
		write(FD, &ap[i], 1);
		i++;
		l--;
	}
}

void		print_zero(int c, t_flags *f)
{
	int		i;

	i = f->weidth - c;
	while (i > 0)
	{
		ft_putchar_fd('0', FD);
		i--;
	}
}

void		print_space(int c, t_flags *f)
{
	int		i;

	i = f->weidth - c;
	while (i > 0)
	{
		ft_putchar_fd(' ', FD);
		i--;
	}
}
