/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_flags.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarsell <dmarsell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/04 17:04:53 by dmarsell          #+#    #+#             */
/*   Updated: 2020/08/06 10:45:24 by dmarsell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int			out_num(const char *format, int size1, int size, int start)
{
	int		l;
	char	*pr;
	int		c;

	l = start + size1 - size;
	pr = ft_strsub(format, size, l);
	c = ft_atoi(pr);
	free(pr);
	return (c);
}

void		other_spec(const char *ft, t_flags *f, int i)
{
	if (ft[i] == '-')
		f->minus = 1;
	if (ft[i] == '+')
		f->plus = 1;
	if (ft[i] == ' ')
		f->space = 1;
	if (ft[i] == '#')
		f->hash = 1;
	if (ft[i] == '*')
		f->star = 1;
	if (ft[i] == '%')
		f->percent = 1;
}

void		fill_flag_next(int i, t_flags *f, const char *fmt, int start)
{
	if (fmt[i] == '.')
	{
		f->dot = 1;
		f->precision = out_num(fmt, f->size, i + 1, start);
	}
	fill_type(fmt, f, start);
	find_weidth(fmt, f, start);
}

int			ft_choice(char c)
{
	if ((c == 'd') || (c == 'i') || (c == 'o') || (c == 'u') ||
	(c == 'x') || (c == 'c') || (c == 's') || (c == 'p') ||
	(c == 'b') || (c == 'f') || (c == 'X') || (c == 'D') ||
	(c == 'I') || (c == 'O') || (c == 'U') || (c == 'C') ||
	(c == 'S') || (c == 'P') || (c == 'F'))
		return (1);
	return (0);
}

void		fill_flag(const char *fmt, t_flags *f, int start)
{
	int		i;

	i = 1 + start;
	while (fmt[i] != '.' && fmt[i] != '\0')
	{
		if (fmt[i] == '0')
		{
			if ((fmt[i + 1] >= '0' && fmt[i + 1] <= '9')\
			&& (fmt[i - 1] < '0' || fmt[i - 1] > '9'))
				f->zero = 1;
			if ((fmt[i + 1] < '0' || fmt[i + 1] > '9')\
			&& (fmt[i - 1] < '0' || fmt[i - 1] > '9'))
				f->zero = 1;
		}
		else if (ft_choice(fmt[i]))
			break ;
		else
			other_spec(fmt, f, i);
		i++;
	}
	fill_flag_next(i, f, fmt, start);
}
