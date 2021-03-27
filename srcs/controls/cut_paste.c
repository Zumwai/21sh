/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cut_paste.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 15:00:42 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 15:55:15 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

void	copy_word(t_term *pos, t_yank *buffer)
{
	int		curr;
	int		end;
	int		abs;

	abs = (pos->delta_x ^ (pos->delta_x >> 31) - (pos->delta_x >> 31));
	if (buffer->yanked)
		set_free_null(&buffer->yanked);
	if (pos->index == 0 || pos->index - abs <= 0)
		return ;
	curr = pos->index - abs - 1;
	end = curr;
	while (curr > 0 && pos->new[curr] == ' ')
		curr--;
	while (curr > 0 && ft_ischar(pos->new[curr]))
		curr--;
	if (!ft_ischar(pos->new[curr]))
		curr++;
	buffer->yanked = ft_strsub(pos->new, curr, end - curr + 1);
}

void	cut_word(t_term *pos, t_yank *buffer)
{
	int	curr;
	int	end;
	int	abs;

	abs = (pos->delta_x ^ (pos->delta_x >> 31) - (pos->delta_x >> 31));
	if (buffer->yanked)
		set_free_null(&buffer->yanked);
	if (pos->index == 0 || pos->index - abs <= 0)
		return ;
	curr = pos->index - abs;
	end = curr;
	while (curr > 0 && pos->new[curr - 1] == ' ')
		curr--;
	while (curr > 0 && ft_ischar(pos->new[curr - 1]))
		curr--;
	buffer->yanked = ft_strsub(pos->new, curr, end - curr);
	ft_memmove(&pos->new[curr], &pos->new[end], pos->index - end);
	pos->index -= (end - curr);
	pos->x -= (end - curr);
	ft_memset(&pos->new[pos->index], 0, end - curr);
}

void	cut_after(t_term *pos, t_yank *buffer)
{
	int	curr;
	int abs;

	abs = (pos->delta_x ^ (pos->delta_x >> 31) - (pos->delta_x >> 31));
	if (buffer->yanked)
		set_free_null(&buffer->yanked);
	curr = pos->index - abs;
	buffer->yanked = ft_strsub(pos->new, curr, abs);
	ft_memset(&pos->new[curr], 0, abs);
	pos->index = curr;
	pos->delta_x = 0;
}

void	cut_before(t_term *pos, t_yank *buffer)
{
	size_t	curr;
	size_t	abs;

	abs = (pos->delta_x ^ (pos->delta_x >> 31) - (pos->delta_x >> 31));
	if (buffer->yanked)
		set_free_null(&buffer->yanked);
	curr = pos->index - abs;
	buffer->yanked = ft_strsub(pos->new, 0, curr);
	ft_memmove(pos->new, &pos->new[curr], abs);
	ft_memset(&pos->new[pos->index - curr], 0, curr);
	pos->index -= (pos->index + pos->delta_x);
	if (!abs && !pos->prev)
		pos->x = pos->index + 7;
	else
		pos->x = 0;
}

void	yank_buffer(t_term *pos, char *line)
{
	int	size;
	int	curr;
	int abs;

	abs = (pos->delta_x ^ (pos->delta_x >> 31) - (pos->delta_x >> 31));
	if (!line)
		return ;
	if (line[0] == 0)
		return ;
	size = ft_strlen(line);
	if (size + pos->index >= ARG_MAX)
		return ;
	if (size + pos->index >= pos->buf_size)
		pos->new = get_buf_line(&pos->new, &pos->buf_size, size);
	curr = pos->index - abs;
	ft_memmove(&pos->new[curr + size], &pos->new[curr], abs);
	ft_memmove(&pos->new[curr], line, size);
	pos->index += size;
	if (pos->delta_x > 0)
		pos->delta_x = 0;
}
