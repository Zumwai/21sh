#include "sh.h"

int		determine_state(char *line, int prev, t_term *pos)
{
	size_t	index = 0;
	int		state;

	state = prev;
	while (line[index])
	{
		if (line[index] == '\'')
		{
			if (state == DEFAULT)
				state = QUOTES;
			else if (state == QUOTES)
				state = DEFAULT;
		}
		if (line[index] == '\"')
		{
			if (state == DEFAULT)
				state = DOUBLE_QUOTES;
			else if (state == DOUBLE_QUOTES)
				state = DEFAULT;
		}
		if (line[index] == '<' && line[index + 1] == '<')
		{
			if (state == DEFAULT)
			{
				pos->heredoc = index + 1;
				state = HEREDOC;
				break ;
			}
		}
		index++;
	}
	if (line[index - 1] == '\\')
		pos->glue = true;
	return (state);
}