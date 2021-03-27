/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 10:18:57 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 10:43:23 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static int		find_max_elem(t_auto *list)
{
	int		size;
	int		max;
	t_auto	*curs;

	curs = list;
	size = 0;
	max = 0;
	while (curs)
	{
		size++;
		if (curs->size > max)
			max = curs->size;
		curs = curs->next;
	}
	max += 1;
	return (max);
}

void			display_vars(t_auto *list, char *orig, int cols, int max)
{
	t_auto	*curs;
	t_auto	*tmp;
	int		len_orig;
	int		i;

	i = 0;
	len_orig = ft_strlen(orig);
	curs = list;
	while (curs)
	{
		tmp = curs;
		curs = curs->next;
		ft_putstr_size(orig, len_orig - 1);
		ft_putstr(tmp->name);
		while (tmp->size++ < max)
			ft_putchar(' ');
		i++;
		if (i == cols)
		{
			ft_putchar('\n');
			i = 0;
		}
		free(tmp->name);
		free(tmp);
	}
}

void			print_varians(t_term *pos, t_auto *list, char *orig)
{
	struct winsize	dimensions;
	t_auto			*curs;
	int				max;
	int				cols;

	curs = list;
	cols = 0;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	max = find_max_elem(list);
	curs = list;
	cols = dimensions.ws_col / max;
	ft_putchar('\n');
	display_vars(list, orig, cols, max);
	ft_putchar('\n');
	update_coord(pos);
}
