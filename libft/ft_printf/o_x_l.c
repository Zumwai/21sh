/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_x_l.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarsell <dmarsell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/06 15:22:59 by dmarsell          #+#    #+#             */
/*   Updated: 2020/08/06 10:47:06 by dmarsell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char		do_16_abc(int t, t_flags *f)
{
	char	ab;
	int		i;

	ab = '\0';
	i = t - 10;
	if (f->type == 'X')
		ab = 'A';
	else
		ab = 'a';
	ab = (ab + i);
	return (ab);
}

char		*fill_l_next(char *dst, int *p, t_flags *f, unsigned int tmp)
{
	if (f->dot && tmp == 0 && f->hash == 0)
		return (dst);
	if (f->hash == 1)
	{
		f->type == 'o' ? dst[0] = '0' : 1;
		if (tmp == 0)
			return (dst);
		if (f->type == 'o')
			*p = 0;
		else
			*p = -1;
	}
	f->hash == 0 ? *p = -1 : 1;
	return (dst);
}

char		*fill_l(char *dst, int c, t_flags *f, unsigned long tmp)
{
	char	t;
	int		p;

	dst = fill_l_next(dst, &p, f, tmp);
	while (c > p)
	{
		if (tmp == 0 && f->hash)
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

char		*l_to_s(unsigned long tmp, t_flags *f)
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
		if (f->hash > 0)
			h = 1;
		else
			h = 0;
		f->div = 8;
	}
	c = how_much_l(tmp, f) + h;
	dst = (char *)malloc(sizeof(char) * (c + 1));
	ft_bzero(dst, c + 1);
	dst = fill_l(dst, c, f, tmp);
	return (dst);
}

int			convert_to_l(unsigned long src, t_flags *f)
{
	int		res;
	char	*rlt;

	rlt = l_to_s(src, f);
	src == 0 && f->type != 'o' ? f->hash = 0 : 1;
	res = print_num_str(rlt, f);
	free(rlt);
	return (res);
}
