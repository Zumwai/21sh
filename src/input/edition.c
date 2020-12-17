#include "shell.h"

void	backspace_char(char *new, t_term *pos)
{
	int abs = ft_abs(pos->delta_x);
	if (!pos->delta_x && pos->index)
	{
		ft_printf("\010 \010");
		new[--pos->index] = '\0';
		pos->x--;
	}
	else if (abs < pos->index)
	{
		ft_memmove(&new[pos->index - abs - 1], &new[pos->index - abs], abs);
		new[--pos->index] = '\0';
		pos->x--;
	}
}

void insert_char (char *new, t_term *pos, char c)
{
	int abs = ft_abs(pos->delta_x);

	ft_memmove(&new[pos->index - abs + 1], &new[pos->index - abs], abs);
	new[pos->index - abs] = c;
	pos->x++;
	pos->index++;
}

void delete_char(char *new, t_term *pos)
{
	int abs = ft_abs(pos->delta_x);
	if (abs == pos->index && pos->index)
	{
		new = ft_memmove(new, &new[1], pos->index - 1);
		pos->index--;
		pos->delta_x++;
	}
	else if (abs && abs < pos->index)
	{
		ft_memmove(&new[pos->index - abs], &new[pos->index - abs + 1], abs);
		pos->index--;
		pos->delta_x++;
	}
}