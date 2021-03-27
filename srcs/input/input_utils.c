/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 19:33:25 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 19:33:25 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "sh.h"

int key_exit(t_term *pos, __attribute((unused))t_yank *buffer)
{
	tcsetattr(STDIN_FILENO, TCSADRAIN, &g_sig.old);
	return(-2);
}

static void get_coordinates(int *start_y, int *start_x)
{
	char	buf[32];
	size_t	index = 0;
	int		red = 0;
	ft_bzero(buf, 32);
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
	bzero(pos, sizeof(t_term));
	ft_memset(pos, 0, sizeof(t_term));
	pos->main = NULL;
	get_coordinates(&pos->y, &pos->x);
	if (!(pos->store = (t_scroll *)malloc(sizeof(t_scroll))))
		handle_exit_errors("Malloc returned NULL");
	bzero(pos->store, sizeof(t_scroll));
	if (!head)
	{
		pos->main = create_main_line();
		bzero(pos->main, sizeof(t_actual));
	}
	else {
		pos->main = *head;
	}
	return (pos);
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

char	*concat_symbol(char *frst, char *scnd, char c)
{
	char	*new;

	new = ft_strnew(ft_strlen(frst) + ft_strlen(scnd) + 2);
	ft_strcpy(new, frst);
	ft_strcat(new, &c);
	ft_strcat(new, scnd);
	return new;
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
