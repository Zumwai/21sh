/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calc_float.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarsell <dmarsell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/07 01:18:51 by dmarsell          #+#    #+#             */
/*   Updated: 2020/08/06 11:42:36 by dmarsell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_itoa_base(long long int value, int base)
{
	unsigned long long int	uvalue;
	char					*rezult;
	int						rezult_size;
	int						sign;

	if (base < 2 || base > 16)
		return ((void *)0);
	sign = value < 0 && base == 10 ? 1 : 0;
	uvalue = value < 0 ? -value : value;
	rezult_size = ft_div_base(uvalue, base);
	rezult_size += sign;
	if (!(rezult = (char *)malloc(sizeof(char) * (rezult_size + 1))))
		return ((void *)0);
	rezult[rezult_size] = '\0';
	if (sign)
		rezult[0] = '-';
	while ((rezult_size - 1) >= sign)
	{
		rezult[rezult_size - 1] = (uvalue % base) >= 10 ?\
		uvalue % base - 10 + 'A' : uvalue % base + '0';
		uvalue /= base;
		rezult_size--;
	}
	return (rezult);
}

int		ft_fract(char (*bufer)[70], long double n, int p)
{
	int					i;
	int					hd;
	unsigned long long	tmp;
	long double			m;

	m = n;
	hd = 0;
	i = 0;
	tmp = 10;
	while (n > 0 && i < 70)
	{
		n = 10 * n;
		(*bufer)[i] = i < 19 ? (unsigned long long)(m * tmp) % 10 : (char)(n);
		i < 19 ? tmp *= 10 : i;
		n = (n - (char)n);
		i++;
	}
	if (i > p)
	{
		hd = ft_circ(bufer, p);
		ft_to_str(bufer, p);
	}
	else
		ft_to_str(bufer, i);
	return (hd);
}

int		ft_precision(int p, t_flags *f)
{
	p == 0 && !(f->dot) ? p = 6 : 1;
	return (p);
}

char	*ft_ftoa(long double n, int p, t_flags *f)
{
	char	*sign;
	char	*entire;
	char	*rezult;
	char	bufer[70];
	int		hd;

	p = ft_precision(p, f);
	sign = n < 0 ? "-" : "";
	n = n < 0 ? -n : n;
	ft_bzero(bufer, 70);
	hd = ft_fract(&bufer, n - (long int)n, p);
	if (!(entire = ft_itoa_base((long int)n + hd, 10)))
		return (0);
	if (!(rezult = ft_str_connect(entire, bufer, sign, p)))
		return (0);
	free(entire);
	return (rezult);
}

int		calc_float(t_flags *f, va_list ap, char *buff, char *str)
{
	union u_longint_double	val;
	int						res;

	res = 0;
	f->varsize = f->size;
	if (f->len[0] == 'L')
		val.double_val = va_arg(ap, long double);
	else
		val.double_val = va_arg(ap, double);
	buff = ft_ftoa(val.double_val, f->precision, f);
	buff ? f->varsize = ft_strlen(buff) : 1;
	if (!(buff))
		return (0);
	if (f->hash && !(ft_strrchr(buff, '.')))
		buff[f->varsize] = '.';
	buff[0] == '-' ? f->decminus = 1 : 1;
	res = print_float(f, buff);
	str = (void *)str;
	free(buff);
	return (res);
}
