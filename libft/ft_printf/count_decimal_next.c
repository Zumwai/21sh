/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_decimal_next.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarsell <dmarsell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/06 11:51:51 by dmarsell          #+#    #+#             */
/*   Updated: 2020/08/06 11:51:52 by dmarsell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_count_num_l(long n, char *buff, int *res)
{
	if (n < 0)
	{
		(*res)++;
		buff[*res - 1] = '-';
		n *= -1;
	}
	if (n >= 10)
		ft_count_num_l(n / 10, buff, res);
	(*res)++;
	buff[*res - 1] = (n % 10) + '0';
}

void	ft_count_num_ll(long long n, char *buff, int *res)
{
	if (n < 0)
	{
		(*res)++;
		buff[*res - 1] = '-';
		n *= -1;
	}
	if (n >= 10)
		ft_count_num_ll(n / 10, buff, res);
	(*res)++;
	buff[*res - 1] = (n % 10) + '0';
}

void	ft_count_num_ui(unsigned int n, char *buff, int *res)
{
	if (n >= 10)
		ft_count_num_ui(n / 10, buff, res);
	(*res)++;
	buff[*res - 1] = (n % 10) + '0';
}

void	ft_count_num_ul(unsigned long n, char *buff, int *res)
{
	if (n >= 10)
		ft_count_num_ul(n / 10, buff, res);
	(*res)++;
	buff[*res - 1] = (n % 10) + '0';
}

void	ft_count_num_ull(unsigned long long n, char *buff, int *res)
{
	if (n >= 10)
		ft_count_num_ull(n / 10, buff, res);
	(*res)++;
	buff[*res - 1] = (n % 10) + '0';
}
