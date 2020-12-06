/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_decimal_next_n.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarsell <dmarsell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 11:53:34 by dmarsell          #+#    #+#             */
/*   Updated: 2020/08/06 11:53:35 by dmarsell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_sp_bit_n(char *tmp, char *buff, int *res, t_count_d *count)
{
	int		j;
	int		i;

	*res = count->start;
	j = 0;
	i = 0;
	while (*res >= 0)
	{
		if (j == 8 && count->space > 0 && *res != count->start && *res != 0)
		{
			buff[i++] = ' ';
			count->space--;
			j = 0;
		}
		buff[i++] = tmp[(*res)--];
		j++;
	}
	buff[i] = '\0';
	*res = i;
}

void	ft_space_bit(char *tmp, char *buff, int *res, t_flags *f)
{
	t_count_d count;

	if (f->len[0] == 'h' && f->len[1] == '\0')
	{
		count.space = 1;
		count.start = 15;
	}
	else if (f->len[0] == 'h' && f->len[1] == 'h')
	{
		count.space = 0;
		count.start = 7;
	}
	else if (f->len[0] == 'l')
	{
		count.space = 7;
		count.start = 63;
	}
	else
	{
		count.space = 7;
		count.start = 63;
	}
	ft_sp_bit_n(tmp, buff, res, &count);
}

void	ft_count_bit(unsigned long long n, char *buff, int *res)
{
	if (n >= 2)
		ft_count_bit(n / 2, buff, res);
	(*res)++;
	buff[*res - 1] = (n % 2) + '0';
}

void	ft_count_num_i(int n, char *buff, int *res)
{
	if (n < 0)
	{
		(*res)++;
		buff[*res - 1] = '-';
		n = n * -1;
	}
	if (n >= 10)
		ft_count_num_i(n / 10, buff, res);
	(*res)++;
	buff[*res - 1] = (n % 10) + '0';
}
