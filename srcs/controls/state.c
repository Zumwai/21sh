#include "sh.h"

char	*search_heredoc(t_term *pos, int padd)
{
	char	*str;
	char	*tmp;
	if (!pos->prev)
		return NULL;
	int		len;
	t_term *curs;
	curs = pos;
	str = NULL;
	while (curs && curs->glue)
	{
		len = ft_strlen(curs->new);
		if (len > 1)
		{
			tmp = ft_strnew(ft_strlen(str) + len + 1);
			ft_strcpy(tmp, curs->new);
			if (str)
				ft_strcat(tmp, str);
			str = tmp;
		}
		curs = curs->prev;
	}
	return str;
}

char	*determine_glue(char *line, t_term *pos, int index)
{
	if (line[index - 1] == '\\') {
		if (pos->prev && pos->prev->glue)
			pos->glue = MID;
		else
			pos->glue = BEG;
	} else {
		if (!pos->prev)
			pos->glue = NONE;
		else if (pos->prev && pos->prev->glue) {
			pos->glue = FIN;
			if (pos->prev->glue == FIN)
				pos->prev->glue = MID;
		}
	}
	if (pos->glue == FIN)
	{
		if (pos->new[0] == '<')
			return (search_heredoc(pos, 1));
		else
			return (search_heredoc(pos, 0));
	}
	return NULL;
}

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

	return (state);
}