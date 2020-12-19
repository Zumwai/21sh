#include "shell.h"

void	cut_word(char *new, t_term *pos)
{
	int		curr;
	int		end;
	int 	abs = ft_abs(pos->delta_x);

	if (pos->yanked)
		set_free_null(pos->yanked);
	if (pos->index == 0)
		return ;
	curr = pos->index - abs - 1;
	end = curr;
	while(curr > 0 && new[curr] == ' ')
		curr--;
	while(curr > 0 && ft_ischar(new[curr]))
		curr--;
	curr++;
	pos->yanked = ft_strsub(new, curr, end - curr);
	ft_memmove(&new[curr], &new[end], pos->index - end);
	pos->index -= (end - curr) + 1;
	ft_memset(&new[pos->index], 0, end - curr);
		/*
	ft_memmove(new, &new[curr], end - curr);
	ft_memset(&new[pos->index - curr], 0, curr);
	pos->index -= (pos->index + pos->delta_x);
	*/
}

void	cut_after(char *new, t_term *pos)
{
	int		curr;
	int 	abs = ft_abs(pos->delta_x);

	if (pos->yanked)
		set_free_null(pos->yanked);
	curr = pos->index - abs;
	pos->yanked = ft_strsub(new, curr, abs);
	ft_memset(&new[curr], 0, abs);
	pos->index = curr;
	pos->delta_x = 0;
}

void	cut_before(char *new, t_term *pos)
{
	int		curr;
	int 	abs = ft_abs(pos->delta_x);
	if (pos->yanked)
		set_free_null(pos->yanked);
	curr = pos->index - abs;
	pos->yanked = ft_strsub(new, 0, curr);
	ft_memmove(new, &new[curr], abs);
	ft_memset(&new[pos->index - curr], 0, curr);
	pos->index -= (pos->index + pos->delta_x);
}

void	yank_buffer(char *new, t_term *pos)
{
	size_t	size;
	int		curr;
	int 	abs = ft_abs(pos->delta_x);

	if (!pos->yanked)
		return ;
	if (pos->yanked[0] == 0)
		return ;
	size = ft_strlen(pos->yanked); //calc at creation time, not here
	if (size + pos->index >= pos->buf_size)
		new = get_buf_line(&new, &pos->buf_size, size);
	curr = pos->index - abs;
	ft_memmove(&new[curr + size], &new[curr], abs);
	ft_memmove(&new[curr], pos->yanked, size);
	pos->index += size;
	pos->delta_x -= size;
}

int 	read_key(char *new, long long key, t_term *pos, struct termios old)
{
			if (key == 27)
				key_exit(old, pos);
			else if (key == BACKSPACE)
				backspace_char(new, pos);
			else if (key == ENTER) //depends on a state
				return (-1);
			else if (key == DELETE)
				delete_char(new, pos);
			else if (key >= 32 && key <= 127)
				insert_char(new, pos, key);
			else if (key == LEFT)
				move_left(pos);
			else if (key == RIGHT)
				move_right(pos);
			else if (key == L_WORD)
				go_prev_word(new, pos);
			else if (key == R_WORD)
				go_next_word(new, pos);
			else if (key == START)
				pos->delta_x = -pos->index;
			else if (key == END)
				pos->delta_x = 0;
			else if (key == L_UP )
				change_line_up(pos);
			else if (key == L_DOWN)
				change_line_down(pos);
			else if (key == R_CUT)
				cut_after(new, pos);
			else if (key == L_CUT)
				cut_before(new, pos);
			else if (key == YANK)
				yank_buffer(new, pos);
			else if (key == W_CUT)
				cut_word(new, pos);
			return (0);
}