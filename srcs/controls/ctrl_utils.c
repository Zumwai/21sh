/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctrl_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 15:38:34 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 17:51:28 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

extern t_term	*get_last_pos(t_term *pos)
{
	t_term *curs;

	curs = pos;
	while (curs->next != NULL)
		curs = curs->next;
	return (curs);
}

extern int		is_printable(char key)
{
	if (key >= 32 && key <= 127)
		return (1);
	if (key == ' ' || key == '\t' || key == '\v' || key == '\f' || key == '\r')
		return (1);
	return (0);
}
