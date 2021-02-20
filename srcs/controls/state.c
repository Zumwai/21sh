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
*/

char	find_last_char(char *str, int i)
{
	if (i < 0)
		return 0;
	while (i > 0)
	{
		if (str[i] > 32 && str[i] < 127)
			return str[i];
		i--;
	}
}

char	find_next_char(char *str, int i)
{
	if (i < 0)
		return 0;
	while (str[i])
	{
		if (str[i] != ' ')
			return str[i];
		i++;
	}
}

int	check_for_zero(char *str, int i)
{
	if (i < 0)
		return 0;
	while (str[i])
	{
		if (str[i] = '\\') {
			if (str[ i + 1] == 0)
				return 1;
			else
				return 0;
		}
			break ;
		i++;
	}
	if (str[i] == 0)
		return 1;
	return 0;
}

int		verify_char_heredoc(char c)
{
	if (c == '|' || c == '&' || c == ';' || c == '\n' || c == 0 || c == '<' || c == '>')
		return 1;
	return 0;
}
int		parse_incoming_subline(char *str, int prev)
{
	int	i = 0;
	int	doc = 0;
	int	state = prev;
	char	c = 0;
	int		flag ;
	state &= ~(GLUE);
	if (!str[0]) /* placeholer for empy line...for now */
		return -1;
	while (str[i]) {
		char b = str[i];
		if ((state & ARG_HDOC))
		{
			c = find_next_char(str, i);
			if (verify_char_heredoc(c))
				return -1;
			else if (c != '\\' && !check_for_zero(str, i)) {
				state ^= ARG_HDOC;
			}
		}
		else if ((state & REQ_HDOC)) {
			if (str[i] == '<') {
				c = find_next_char(str, i + 1);
				if (verify_char_heredoc(c))
					return -1;
				else if (c == '\\' && check_for_zero(str, i + 1)) {
					state ^= ARG_HDOC;
					state ^= REQ_HDOC;
				} else {
					state ^= ARG_HDOC;
					state ^= REQ_HDOC;
				}
				if (str[1] == 0)
					return -1;
			} else if (i > 0 && str[i] == '<' && str[i - 1] == '<' && !(state & QUOTE) && !(state & D_QUOTE)) {
				state ^= ARG_HDOC;
				state ^= REQ_HDOC;
			} else if (i == 0 && str[0] != '\\' && str[1])
				return -1;
			else if (str[i] != '<' && str[i + 1] != '\\' && str[i + 2] != 0)
				return -1;
		}
		else if (str[i] == '\'' && !(state & D_QUOTE))
			state ^= (QUOTE);
		else if (str[i] == '\"'	&& !(state & QUOTE))
		state ^= (D_QUOTE);
		else if (!state && str[i] == '<') {
			doc++;
			state |= REQ_HDOC;
		}
		else if (!state && doc == 2) {
			doc = 0;
			state ^= REQ_HDOC;
			state |= ARG_HDOC;
		}
		i++;
	} 
	i--;
	if (!(state & QUOTE) && str[i] == '\\')
		state ^= (GLUE);
	return state;
}


int		consult_state(t_term *curs)
{
	int		ret = 0;
	int		small = 0;
	if (curs->prev)
		if (!curs->main)
			curs->main = create_main_line();
	small = parse_incoming_subline(curs->new, curs->main->state);
	//ret = curs->main->state ^ small;
	return ret;
}