/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/25 16:17:55 by aophion           #+#    #+#             */
/*   Updated: 2019/09/25 16:19:29 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdel(t_liblist **alst, void (*del)(void *, size_t))
{
	t_liblist		*temp;

	if (alst && del)
	{
		if (*alst)
		{
			while (*alst)
			{
				temp = (*alst)->next;
				ft_lstdelone(alst, del);
				*alst = temp;
			}
			alst = NULL;
		}
	}
}
