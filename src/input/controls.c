#include "shell.h"

void	cut_word(t_term *pos, t_yank *buffer)
{
	int		curr;
	int		end;
	int 	abs = ft_abs(pos->delta_x);

	if (buffer->yanked)
		set_free_null(buffer->yanked);
	if (pos->index == 0)
		return ;
	curr = pos->index - abs - 1;
	end = curr;
	while(curr > 0 && pos->new[curr] == ' ')
		curr--;
	while(curr > 0 && ft_ischar(pos->new[curr]))
		curr--;
	curr++;
	buffer->yanked = ft_strsub(pos->new, curr, end - curr);
	ft_memmove(&pos->new[curr], &pos->new[end], pos->index - end);
	pos->index -= (end - curr) + 1;
	ft_memset(&pos->new[pos->index], 0, end - curr);
		/*
	ft_memmove(pos->new, &pos->new[curr], end - curr);
	ft_memset(&pos->new[pos->index - curr], 0, curr);
	pos->index -= (pos->index + pos->delta_x);
	*/
}

void	cut_after(t_term *pos, t_yank *buffer)
{
	int		curr;
	int 	abs = ft_abs(pos->delta_x);

	if (buffer->yanked)
		set_free_null(buffer->yanked);
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
		set_free_null(buffer->yanked);
	curr = pos->index - abs;
	buffer->yanked = ft_strsub(pos->new, 0, curr);
	ft_memmove(pos->new, &pos->new[curr], abs);
	ft_memset(&pos->new[pos->index - curr], 0, curr);
	pos->index -= (pos->index + pos->delta_x);
}

void	yank_buffer(t_term *pos, t_yank *buffer)
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
		pos->new = get_buf_line(&pos->new, &pos->buf_size, size); //pointer loss, same problem with history
	curr = pos->index - abs;
	ft_memmove(&pos->new[curr + size], &pos->new[curr], abs);
	ft_memmove(&pos->new[curr], buffer->yanked, size);
	pos->index += size;
//	pos->delta_x += size;
	if (pos->delta_x >0)
		pos->delta_x = 0;
//	pos->x += size;
}

void	ft_clear(t_term *pos)
{
		ft_printf("%s", "\033[2J");
		tputs (tgoto (tgetstr("cm", NULL), 0, 0), 1, putchar_like);
		pos->y = 0;
}

int		consult_state(long long key, t_term *pos)
{
	if (!pos->state)
	{
		ft_putchar_fd((char)key, 1);
		return (-1);
	}
	if (pos->state == 1)
	{
		insert_char(pos, (char)key);
		return (0);
	}
	return (1);
}

void	move_home(t_term *pos)
{
	pos->delta_x = -pos->index;
}

void	move_end(t_term *pos)
{
	pos->delta_x = 0;
}

int 	read_key(long long key, t_term *pos, struct termios old, t_yank *buf)
{
			if (key == 27)
				return (key_exit(old, pos, buf));
			else if (key == ENTER)
				return (consult_state(key, pos));
			else if (key == BACKSPACE)
				backspace_char(pos);
			else if (key == DELETE)
				delete_char(pos);
			else if (key >= 32 && key <= 127)
				insert_char(pos, key);
			else if (key == LEFT)
				move_left(pos);
			else if (key == RIGHT)
				move_right(pos);
			else if (key == L_WORD)
				go_prev_word(pos);
			else if (key == R_WORD)
				go_next_word(pos);
			else if (key == START)
				move_home(pos);
			else if (key == END)
				move_end(pos);
			else if (key == L_UP )
				change_line_up(pos);
			else if (key == L_DOWN)
				change_line_down(pos);
			else if (key == R_CUT)
				cut_after(pos, buf);
			else if (key == L_CUT)
				cut_before(pos, buf);
			else if (key == YANK)
				yank_buffer(pos, buf);
			else if (key == W_CUT)
				cut_word(pos, buf);
			else if (key == CLEAR)
				ft_clear(pos);
			return (0);
}