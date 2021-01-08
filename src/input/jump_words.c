#include "shell.h"

void go_next_word(t_term *pos)
{
	int abs = ft_abs(pos->delta_x);

	int curs = pos->index - abs;
	while (pos->new[curs] != '\0' && !ft_isalnum(pos->new[curs]))
		curs++;
	while (pos->new[curs] && ft_isalnum(pos->new[curs]))
		curs++;
	curs = pos->index - curs;
	pos->delta_x = -curs;
	if (pos->delta_x > 0)
		pos->delta_x = 0;
}

void go_prev_word(t_term *pos)
{
	int abs = ft_abs(pos->delta_x);

	int curs = pos->index - abs - 1;
	while (curs > 0 && pos->new[curs] == ' ')
		curs--;
	while (curs > 0 && ft_isalnum(pos->new[curs]))
		curs--;
	if (curs > 0)
	{
		curs++;
		curs = pos->index - curs;
		pos->delta_x = -curs;
	}
	else
		pos->delta_x = -pos->index;
}
