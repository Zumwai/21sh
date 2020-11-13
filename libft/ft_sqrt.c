/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sqrt.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/14 17:10:03 by aophion           #+#    #+#             */
/*   Updated: 2019/09/25 15:23:20 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int		ft_sqrt(int nb)
{
	int		check;

	check = 1;
	if (nb == 0)
		return (0);
	if (nb < 0)
		return (0);
	while (check < nb / 2)
	{
		if (check * check == nb)
			return (check);
		else
			check++;
	}
	return (0);
}
