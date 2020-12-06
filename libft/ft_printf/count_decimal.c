/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_decimal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarsell <dmarsell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 11:51:35 by dmarsell          #+#    #+#             */
/*   Updated: 2020/08/06 11:51:36 by dmarsell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_bit_add_zero(char *buff, int *res, t_flags *f)
{
	char	tmp[73];
	int		i;

	(*res)--;
	i = 0;
	while (*res >= 0)
		tmp[i++] = buff[(*res)--];
	while (i < 72)
		tmp[i++] = '0';
	tmp[i] = '\0';
	ft_space_bit(tmp, buff, res, f);
}

int		count_dec_n_n_n(t_flags *f, char *buff, unsigned long long i, int *res)
{
	if (f->type == 'u')
		ft_count_num_ui((unsigned int)i, buff, res);
	else
	{
		if ((long long)i == -2147483648 || i == 2147483648U)
		{
			buff = ft_memcpy(buff, "-2147483648", \
			ft_strlen("-2147483648") + 1);
			*res = ft_strlen(buff);
		}
		else
			ft_count_num_i((int)i, buff, res);
	}
	return (*res);
}

int		count_dec_n_n(t_flags *f, char *buff, unsigned long long i, int *res)
{
	if (f->len[0] == 'l' && f->len[1] == 'l')
	{
		if (f->type == 'u')
			ft_count_num_ull(i, buff, res);
		else
		{
			if (i == -9223372036854775808U || i == 9223372036854775808U)
			{
				buff = ft_memcpy(buff, "-9223372036854775808", \
				ft_strlen("-9223372036854775808") + 1);
				*res = ft_strlen(buff);
			}
			else
				ft_count_num_ll((long long)i, buff, res);
		}
	}
	return (*res);
}

int		count_dec_next(t_flags *f, char *buff, unsigned long long i, int *res)
{
	if (f->len[0] == 'h' && f->len[1] == 'h')
	{
		if (f->type == 'u')
			ft_count_num_ui((unsigned char)i, buff, res);
		else
			ft_count_num_i((char)i, buff, res);
	}
	else if (f->len[0] == 'l' && f->len[1] == '\0')
	{
		if (f->type == 'u')
			ft_count_num_ul((unsigned long)i, buff, res);
		else
		{
			if (i == -9223372036854775808U || i == 9223372036854775808U)
			{
				buff = ft_memcpy(buff, "-9223372036854775808", \
				ft_strlen("-9223372036854775808") + 1);
				*res = ft_strlen(buff);
			}
			else
				ft_count_num_l((long)i, buff, res);
		}
	}
	return (*res);
}

int		count_decimal(t_flags *f, va_list ap, char *buff)
{
	int						res;
	unsigned long long		i;

	res = 0;
	i = va_arg(ap, unsigned long long);
	if (f->type == 'b')
	{
		ft_count_bit(i, buff, &res);
		buff[res] = '\0';
		ft_bit_add_zero(buff, &res, f);
	}
	else if (f->len[0] == 'h' && f->len[1] == '\0')
	{
		if (f->type == 'u')
			ft_count_num_ui((unsigned short)i, buff, &res);
		else
			ft_count_num_i((short)i, buff, &res);
	}
	!res ? count_dec_next(f, buff, i, &res) : res;
	!res ? count_dec_n_n(f, buff, i, &res) : res;
	!res ? count_dec_n_n_n(f, buff, i, &res) : res;
	return (res);
}
