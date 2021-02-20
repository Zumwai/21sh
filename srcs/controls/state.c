#include "sh.h"
/*
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
			int len2 = 0;
			if (str)
				len2 = ft_strlen(str);
			tmp = ft_strnew(len2 + len + 1);
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
*/
int		parse_incoming_subline(char *str)
{
	int	i = 0;
	int	doc = 0;
	int	state = 0;

	while (str[i]) {
		if (str[i] == '\'')
			state ^= (QUOTE);
		if (str[i] == '\"')
			state ^= (D_QUOTE);
		if (!state && str[i] == '<')
			doc++;
		if (!state && doc == 2) {
			doc = 0;
			state |= HEREDOC;
		i++;
	}
	i--;
	if (str[i] == '\\')
		state |= (GLUE);
	if (!(state & QUOTE) && !(state & (D_QUOTE))) {
		if (state & GLUE)
			if (i > 0 && str[i - 1] == '<')
				state |= (REQ_HDOC);
	}
	return state;
}
int		consult_state(t_term *curs)
{
	int		ret = 0;
	int		small = 0;
	if (curs->prev)
		if (curs->main)
			curs->main = create_main_line();
	small = parse_incoming_subline(curs->new);
	//ret = curs->main->state ^ small;
	curs->main->state = ret;
	return ret;
}