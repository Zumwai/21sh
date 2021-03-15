#include "sh.h"

char	*get_incomplete(t_term *pos, int *cd)
{
	int		curr;
	int		end;
	int 	abs = ft_abs(pos->delta_x);
	char	*incomplete;
	char	*test = NULL;
    incomplete = NULL;
	if (pos->index == 0 || pos->index - abs <= 0)
		return NULL;
	curr = pos->index - abs;
	end = curr;
	//if (curr > 0 && pos->new[curr -1] == ' ')
	//	return NULL;
	while(curr > 0 && ft_ischar(pos->new[curr - 1]) && (pos->new[curr - 1] != ';' || pos->new[curr - 1] != '|' || pos->new[curr] != '&'))
		curr--;
	incomplete = ft_strsub(pos->new, curr, end - curr);
    if (curr > 0)
    {
		*cd = 0;
        while (curr >0 && pos->new[curr -1] == ' ')
            curr--;
        end = curr;
        while(curr > 0 && ft_ischar(pos->new[curr - 1]))
		curr--;
		if (curr != end)
			*cd = 1;
    }
	return incomplete;
}
/*
char	*get_incomplete(t_term *pos, int *cd)
{

}
*/
