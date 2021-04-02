/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_inits.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 12:49:25 by aophion           #+#    #+#             */
/*   Updated: 2021/03/29 12:50:15 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void get_coordinates(int *start_y, int *start_x)
{
	char	buf[32];
	size_t	index = 0;
	int		red = 0;

	ft_bzero(buf, 32);
	tcsetattr(STDIN_FILENO, TCSANOW, &(g_sig).work);
	if (write(1, "\033[6n", 4) != 4)
		handle_exit_errors("Write returns Error");
	while (index < sizeof(buf) - 1)
	{
		if (read(STDIN_FILENO, buf + index, 1) != 1)
			break ;
		if (buf[index] == 'R')
			break ;
		index++;
	}
	buf[red] ='\0';
	if (!ft_isdigit(*(buf + 2)))
		return ;
	*start_y = ft_atoi(buf + 2);
	*start_x = 0;
}

t_term	*create_new_io_struct(t_actual **head)
{
	t_term			*pos;
	if (!(pos = (t_term *)malloc(sizeof(t_term))))
		handle_exit_errors("Malloc returned NULL");
	ft_memset(pos, 0, sizeof(t_term));
	pos->main = NULL;
	get_coordinates(&pos->y, &pos->x);
	if (!(pos->store = (t_scroll *)malloc(sizeof(t_scroll))))
		handle_exit_errors("Malloc returned NULL");
	ft_bzero(pos->store, sizeof(t_scroll));
	if (!head)
	{
		pos->main = create_main_line();
		ft_bzero(pos->main, sizeof(t_actual));
	}
	else {
		pos->main = *head;
	}
	return (pos);
}

void       update_coord(t_term *pos)
{
	int	x = 0;
	int y = 0;
	t_term *curs;

	curs = pos;
	get_coordinates(&y, &x);
	while (curs)
	{
		curs->x = x;
		curs->y = y;
		curs = curs->next;
	}
	curs = pos;
	while (curs)
	{
		curs->x = x;
		curs->y = y;
		curs = curs->prev;
	}
}
