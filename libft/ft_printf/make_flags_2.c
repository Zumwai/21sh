/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_flags_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarsell <dmarsell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/05 03:52:05 by dmarsell          #+#    #+#             */
/*   Updated: 2020/08/06 10:46:05 by dmarsell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void		in_precision(const char *ft, t_flags *f, int i)
{
	char	*r;
	int		a;

	a = 0;
	if (f->type == '%')
		i++;
	while (ft[i] != f->type)
	{
		if (ft[i] < '1' || '9' < ft[i])
			i++;
		if (ft[i] >= '1' && ft[i] <= '9')
		{
			if ((r = ft_strsub(ft, i, f->size)))
				a = 1;
			if (r[0] == '0' && r[1] == '\0')
				f->weidth = '0';
			else
			{
				f->weidth = ft_atoi(r);
				free(r);
			}
			break ;
		}
	}
}

void		out_precision(const char *ft, t_flags *f, int i)
{
	char	*r;
	int		a;

	a = 0;
	while (ft[i] != '.')
	{
		if (ft[i] <= '0' || '9' < ft[i])
			i++;
		if (ft[i] >= '1' && ft[i] <= '9')
		{
			if ((r = ft_strsub(ft, i, f->size)))
				a = 1;
			if (r[0] == '0' && r[1] == '\0')
				f->weidth = '0';
			else
			{
				f->weidth = ft_atoi(r);
				free(r);
			}
			break ;
		}
	}
}

void		find_weidth(const char *ft, t_flags *f, int start)
{
	int		i;

	i = start;
	if (f->precision > 0)
		out_precision(ft, f, i);
	if (f->precision == 0)
		in_precision(ft, f, i);
}

void		fill_len(const char *ft, t_flags *tm, int c)
{
	char	comp;

	tm->len = (char *)malloc(sizeof(char) * 3);
	only_zero(tm->len);
	if (ft[c] == 'h' || ft[c] == 'l')
	{
		if (ft[c] == 'h')
			(comp = 'h');
		else
			(comp = 'l');
		tm->len[0] = comp;
		if (ft[c - 1] == comp)
			(tm->len[1] = comp);
		else
			(tm->len[1] = '\0');
	}
	else if (ft[c] == 'L')
	{
		tm->len[0] = 'L';
		tm->len[1] = '\0';
	}
}

void		fill_type(const char *ft, t_flags *tmp, int start)
{
	int		c;
	char	a;

	c = start + tmp->size;
	a = ft[c];
	tmp->type = a;
	if ((tmp->type == '\0' && tmp->percent) || \
	(tmp->type == '\0' && ft[c - 1] == '%'))
		tmp->type = '%';
	if (ft[c - 1] == '*')
		tmp->star = 1;
	if (tmp->type == 'D' || tmp->type == 'I' \
	|| tmp->type == 'O' || tmp->type == 'U' \
	|| tmp->type == 'C' || tmp->type == 'S' \
	|| tmp->type == 'P' || tmp->type == 'F')
		tmp->print = 0;
	else
		tmp->print = 1;
	fill_len(ft, tmp, c - 1);
}
