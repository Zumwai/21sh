/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/25 16:43:29 by aophion           #+#    #+#             */
/*   Updated: 2019/09/25 19:36:58 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_liblist	*ft_lstmap(t_liblist *lst, t_liblist *(*f)(t_liblist *elem))
{
	t_liblist		*new;

	new = NULL;
	if (!lst || !f)
		return (NULL);
	if (!(new = (t_liblist *)malloc(sizeof(t_liblist))))
		return (NULL);
	new = f(lst);
	new->next = ft_lstmap(lst->next, f);
	return (new);
}
