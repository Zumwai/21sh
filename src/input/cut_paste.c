#include "shell.h"

void	copy_word(t_term *pos, t_yank *buffer)
{
	int		curr;
	int		end;
	int 	abs = ft_abs(pos->delta_x);

	if (buffer->yanked)
		set_free_null(&buffer->yanked);
	if (pos->index == 0 || pos->index - abs <= 0)
		return ;
	curr = pos->index - abs - 1;
	end = curr;
	while(curr > 0 && pos->new[curr] == ' ')
		curr--;
	while(curr > 0 && ft_ischar(pos->new[curr]))
		curr--;
	if (!ft_ischar(pos->new[curr]))
		curr++;
	buffer->yanked = ft_strsub(pos->new, curr, end - curr + 1);
}

void	cut_word(t_term *pos, t_yank *buffer)
{
	int		curr;
	int		end;
	int 	abs = ft_abs(pos->delta_x);

	if (buffer->yanked)
		set_free_null(&buffer->yanked);
	if (pos->index == 0 || pos->index - abs <= 0)
		return ;
	curr = pos->index - abs;
	end = curr;
	while(curr > 0 && pos->new[curr - 1] == ' ')
		curr--;
	while(curr > 0 && ft_ischar(pos->new[curr - 1]))
		curr--;
//	curr++;
	buffer->yanked = ft_strsub(pos->new, curr, end - curr);
	ft_memmove(&pos->new[curr], &pos->new[end], pos->index - end);
	pos->index -= (end - curr);
	pos->x -= (end - curr);
	ft_memset(&pos->new[pos->index], 0, end - curr);
}

void	cut_after(t_term *pos, t_yank *buffer)
{
	int		curr;
	int 	abs = ft_abs(pos->delta_x);

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
	int		curr;
	int 	abs = ft_abs(pos->delta_x);
	if (buffer->yanked)
		set_free_null(&buffer->yanked);
	curr = pos->index - abs;
	buffer->yanked = ft_strsub(pos->new, 0, curr);
	ft_memmove(pos->new, &pos->new[curr], abs);
	ft_memset(&pos->new[pos->index - curr], 0, curr);
	pos->index -= (pos->index + pos->delta_x);
	if (!abs)
		pos->x = pos->index + pos->prompt;
}

void	yank_buffer(t_term *pos, t_yank *buffer) //overflow multiple pastes + lines
{
	int	size;
	int		curr;
	int 	abs = ft_abs(pos->delta_x);

	if (!buffer->yanked)
		return ;
	if (buffer->yanked[0] == 0)
		return ;
	size = ft_strlen(buffer->yanked); //calc at creation time, not here
	if (size + pos->index >= pos->buf_size)
		pos->new = get_buf_line(&pos->new, &pos->buf_size, size);
	curr = pos->index - abs;
	ft_memmove(&pos->new[curr + size], &pos->new[curr], abs);
	ft_memmove(&pos->new[curr], buffer->yanked, size);
	pos->index += size;
	if (pos->delta_x > 0)
		pos->delta_x = 0;
}