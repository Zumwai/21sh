/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/24 20:40:10 by aophion           #+#    #+#             */
/*   Updated: 2019/09/25 18:06:03 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_liblist	*ft_lstnew(void const *content, size_t content_size)
{
	t_liblist	*new;

	if (!(new = (t_liblist *)malloc(sizeof(t_liblist))))
		return (NULL);
	if (content)
	{
		if (!(new->content = malloc(content_size)))
		{
			free(new);
			new = NULL;
			return (new);
		}
		ft_memcpy(new->content, content, content_size);
		new->content_size = content_size;
	}
	else
	{
		new->content = NULL;
		new->content_size = 0;
	}
	new->next = NULL;
	return (new);
}
