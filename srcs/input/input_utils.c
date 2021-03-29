/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 19:33:25 by aophion           #+#    #+#             */
/*   Updated: 2021/03/29 12:51:14 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

int key_exit(t_term *pos, t_yank *buffer)
{
	tcsetattr(STDIN_FILENO, TCSADRAIN, &g_sig.old);
	return(-2);
}

char	*ft_strdup_size(char *old, size_t size)
{
	char	*new;

	if (!(new = ft_strnew(size)))
		handle_exit_errors("Malloc returned NULL");
	ft_strcpy(new,old);
	return (new);
}

t_actual	*create_main_line(void)
{
	t_actual	*new;

	if (!(new = (t_actual *)malloc(sizeof(t_actual))))
		handle_exit_errors("Malloc returned NULL");
	new->hdoc = NULL;
	new->line = NULL;
	new->state = 0;
	return new;
}

int		putchar_like(int n)
{
	size_t	size;

	size = write(STDIN_FILENO, &n, 1);
	return (size);
}

void ft_putstr_size(char *line, ssize_t size)
{
	if (size > 0)
		write(1, line, size);
}

void	free_storage(t_term *curr)
{
	int	i;

	i = 0;
	while (curr->store->arr[i])
	{
		free(curr->store->arr[i]);
		curr->store->arr[i] = NULL;
		i++;
	}
	if (curr->store->arr)
		free(curr->store->arr);
	curr->store->size = 0;
	curr->delta_y = 0;
	curr->store->arr = NULL;
}

void correct_y(t_term *pos)
{
	t_term *curs = pos;

	while (curs)
	{
		curs->y--;
		curs = curs->prev;
	}
	curs = pos->next;
	while (curs)
	{
		curs->y--;
		curs = curs->next;
	}
}
