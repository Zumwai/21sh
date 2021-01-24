/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/25 16:32:37 by aophion           #+#    #+#             */
/*   Updated: 2019/09/25 18:31:25 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_liblist *lst, void (*f)(t_liblist *elem))
{
	t_liblist			*cursor;

	if (lst && f)
	{
		cursor = lst;
		while (cursor)
		{
			f(cursor);
			cursor = cursor->next;
		}
	}
}
