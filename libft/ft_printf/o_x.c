/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_x.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarsell <dmarsell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/06 15:02:35 by dmarsell          #+#    #+#             */
/*   Updated: 2020/08/06 10:46:36 by dmarsell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char		*fill_s_next(char *dst, int *p, t_flags *f, unsigned int tmp)
{
	if (f->dot && tmp == 0 && f->hash == 0)
		return (dst);
	if (f->hash == 1)
	{
		f->type == 'o' ? dst[0] = '0' : 1;
		if (tmp == 0 && f->dot && (f->type == 'x' || f->type == 'X'))
			return (dst);
		if (f->type == 'o')
			*p = 0;
		else
			*p = -1;
	}
	if (f->hash == 0 || ((f->type == 'x' || f->type == 'X') \
	&& tmp == 0 && f->dot == 0))
		*p = -1;
	return (dst);
}

char		*fill_s(char *dst, int c, t_flags *f, unsigned int tmp)
{
	char	t;
	int		p;

	dst = fill_s_next(dst, &p, f, tmp);
	while (c > p)
	{
		if (tmp == 0 && f->hash && f->type == 'o')
			break ;
		t = tmp % f->div;
		t >= 10 ? dst[c] = do_16_abc(t, f) : 1;
		if (t < 10)
		{
			t = t + 48;
			dst[c] = (char)t;
		}
		tmp = tmp / f->div;
		c--;
	}
	return (dst);
}

char		*to_s(unsigned int tmp, t_flags *f)
{
	int		c;
	char	*dst;
	int		h;

	if (f->type == 'x' || f->type == 'X')
	{
		f->div = 16;
		h = 0;
	}
	if (f->type == 'o')
	{
		f->div = 8;
		if (f->hash > 0)
			h = 1;
		else
			h = 0;
	}
	c = how_much(tmp, f) + h;
	dst = (char *)malloc(sizeof(char) * c);
	dst[c] = '\0';
	c--;
	dst = fill_s(dst, c, f, tmp);
	return (dst);
}

int			convert_to(unsigned int ap, t_flags *f)
{
	int		res;
	char	*rlt;

	rlt = to_s(ap, f);
	ap == 0 && f->type != 'o' ? f->hash = 0 : 1;
	res = print_num_str(rlt, f);
	free(rlt);
	return (res);
}

int			convert(va_list ap, t_flags *f)
{
	int		res;
	int		i;
	long	k;

	if ((f->type == 'x' || f->type == 'X' || f->type == 'o')
	&& (f->len[0] == '\0' || f->len[0] == 'h'))
	{
		i = va_arg(ap, unsigned int);
		f->len[0] == 'h' && f->len[1] == 'h' ? i = (unsigned char)i : 1;
		f->len[0] == 'h' && f->len[1] == '\0' ? i = (unsigned short)i : 1;
		res = convert_to(i, f);
	}
	if ((f->type == 'x' || f->type == 'X' || f->type == 'o') \
	&& f->len[0] == 'l')
	{
		k = va_arg(ap, unsigned long);
		res = convert_to_l(k, f);
	}
	return (res);
}
