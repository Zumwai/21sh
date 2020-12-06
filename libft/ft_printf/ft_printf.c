/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarsell <dmarsell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/04 16:50:14 by dmarsell          #+#    #+#             */
/*   Updated: 2020/08/06 10:44:44 by dmarsell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		print_persent(t_flags *f)
{
	char	sym;
	int		res;

	res = 0;
	sym = ' ';
	f->zero && f->minus == 0 ? sym = '0' : 1;
	if (f->weidth)
	{
		f->minus ? ft_putchar_fd('%', FD) : 1;
		while (res < f->weidth - 1)
		{
			ft_putchar_fd(sym, FD);
			res++;
		}
		f->minus == 0 ? ft_putchar_fd('%', FD) : 1;
		return (res + 1);
	}
	!(f->weidth) ? ft_putchar_fd('%', FD) : 1;
	return (1);
}

int		to_arg(t_flags *f, va_list ap, char *str)
{
	int			res;
	char		buff[73];

	res = 0;
	if (f->type == '%')
		res = print_persent(f);
	if (f->type == 's' || f->type == 'c' || f->type == 'C' || f->type == 'S')
		res = print_ch(f, ap);
	if (f->type == 'p' || f->type == 'P')
		res = print_address(f, ap);
	if (f->type == 'd' || f->type == 'i' || f->type == 'u' || f->type == 'b'
	|| f->type == 'D' || f->type == 'I' || f->type == 'U')
	{
		res = count_decimal(f, ap, buff);
		buff[res] = '\0';
		res = print_char_decimal(f, buff);
		res++;
	}
	if (f->type == 'o' || f->type == 'x' || f->type == 'X' || f->type == 'O')
		res = convert(ap, f);
	if (f->type == 'f' || f->type == 'F')
		res = calc_float(f, ap, buff, str);
	return (res);
}

void	to_null(t_flags *f, const char *ft, int start)
{
	f->varsize = '\0';
	f->minus = '\0';
	f->plus = '\0';
	f->space = '\0';
	f->zero = '\0';
	f->hash = '\0';
	f->weidth = '\0';
	f->precision = '\0';
	f->div = '\0';
	f->dot = '\0';
	f->star = '\0';
	f->print = '\0';
	f->type = '\0';
	f->decminus = '\0';
	f->len = NULL;
	f->ox = NULL;
	fill_flag(ft, f, start);
}

int		find_le(const char *ft, int st)
{
	while ((ft[st] != 'd') && (ft[st] != 'i') && (ft[st] != 'o')\
	&& (ft[st] != 'u') && (ft[st] != 'x') && (ft[st] != 'c') && \
	(ft[st] != 's') && (ft[st] != 'p') && (ft[st] != 'b') && \
	(ft[st] != 'f') && (ft[st] != 'D') && (ft[st] != 'I') && \
	(ft[st] != 'O') && (ft[st] != 'U') && (ft[st] != 'X') && \
	(ft[st] != 'C') && (ft[st] != 'S') && (ft[st] != 'P') && \
	(ft[st] != 'F') && (ft[st] != '\0'))
		st++;
	return (st);
}

int		all_formats(const char *ft, int i, va_list ap)
{
	t_flags		*f;
	char		*str;
	int			st;
	int			res;

	str = NULL;
	st = i;
	res = 0;
	if (ft[i + 1] == '\0')
		return (0);
	if (!(f = (t_flags *)malloc(sizeof(t_flags))))
		return (0);
	f->size = find_le(ft, st) - i;
	to_null(f, ft, st);
	str = ft_strsub(ft, st + 1, f->size);
	res = to_arg(f, ap, str);
	free(str);
	free(f->len);
	free(f);
	return (res);
}
