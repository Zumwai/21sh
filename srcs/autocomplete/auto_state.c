#include "sh.h"

int		is_delimeter(char c)
{
	if (c == ';' || c == '&' || c == '\"' || c =='\'' || c == '\t'  || c == '\v' || c == '\n' || c == '|' || c == '&')
		return 1;
	return 0;
}

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
	while(curr > 0 && ft_ischar(pos->new[curr - 1]) && pos->new[curr - 1] != ';' && pos->new[curr - 1] != '|' && pos->new[curr] != '&')
		curr--;
	incomplete = ft_strsub(pos->new, curr, end - curr);
	curr--;
	while (curr > 0)
	{
		*cd = 0;
		if (pos->new[curr] == ' ' || pos->new[curr] == '\t' || pos->new[curr] == '\v')
			curr--;
		else if (is_delimeter(pos->new[curr]))
		{
			*cd = 0;
			break ;
		}
		else if (ft_isascii(pos->new[curr]))
		{
			*cd = 1;
			break ;
		}
		else
			curr--;
	}
	/*
    if (curr > 0)
    {
		curr--;
		*cd = 0;
        while (curr >0 && pos->new[curr] == ' ')
            curr--;
        end = curr;
        while(curr > 0 && !is_delimeter(pos->new[curr]))
			curr--;
		if (curr != end)
			*cd = 1;
    }
	*/
	return incomplete;
}
/*
char	*get_incomplete(t_term *pos, int *cd)
{

}
*/
