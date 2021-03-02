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
	while(curr > 0 && pos->new[curr - 1] == ' ')
		curr--;
	while(curr > 0 && ft_ischar(pos->new[curr - 1]))
		curr--;
	incomplete = ft_strsub(pos->new, curr, end - curr);
    if (curr > 0)
    {
		*cd = 1;
        while (curr >0 && pos->new[curr -1] == ' ')
            curr--;
        end = curr;
        while(curr > 0 && ft_ischar(pos->new[curr - 1]))
		curr--;
        test = ft_strsub(pos->new, curr, end - curr);
		if (ft_strequ("cd", test))
			*cd = DIRECTORY;
		free(test);
    }
	return incomplete;
}