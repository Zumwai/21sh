#include "sh.h"

char						get_manage(char t)
{
	if (t == '0')
		return ('0');
	if (t == 't')
		return ('\t');
	if (t == 'n')
		return ('\n');
	if (t == '\\')
		return ('\\');
	else
		return ('\\');
}

static char 				*get_data(char *line, int *n, t_flag *flag)
{
	char 			res[1000];
	int 			j;
	char			*ret;
	char			*buf;

	j = 0;
	while (line[*n] != '\0')
	{
		while (line[*n] != '|' && line[*n] != ';' && line[*n] != '&' && line[*n] != '>' &&
			   line[*n] != '<'  && line[*n] != '\0')
		{
			update_flag(flag, line[*n]);
			if (line[*n] == 92 && line[*n + 1])
			{
				res[j] = get_manage(line[*n + 1]);
				if (res[j] != '\\')
					*n = *n + 1;
			}
			else
				res[j] = line[*n];
			*n += 1;
			j += 1;
		}
		if (line[*n] == '|' || line[*n] == ';' || line[*n] == '&' || line[*n] == '>' ||
		line[*n] == '<' || line[*n] == '\0')
		{
			res[j] = line[*n];
			if (semantica(flag, line, n, &j) == 0)
				continue ;
			if (semantica(flag, line, n, &j) == 1 && j != 0)
			{
				res[j] = '\0';
				buf = ft_strdup(res);
				ret = ft_strtrim(buf);
				free(buf);
				return (ret);
			}
			if (j == 0)
			{
				ret = get_semantica_ret(line, n, res, j);
				*n += 1;
				return (ret);
			}
		}
	}
	buf = ft_strdup(res);
	ret = ft_strtrim(buf);
	free(buf);
	return (ret);
}

extern t_token 			*parsing_t(char *line)
{
	t_token *token;
	int car;
	t_flag *flag;
	int l;
	t_token *cur;

	car = 0;
	if (!(l = ft_strlen(line)))
		return NULL;
	token = init_token();
	cur = token;
	flag = init_flag();
	token->data = get_data(line, &car, flag);
	while (car < l)
	{
		token->next = init_token();
		token->next->prev = token;
		token = token->next;
		token->data = get_data(line, &car, flag);
		if (ft_strcmp(token->data, "") == 0)
			token->data = get_data(line, &car, flag);
		flag = reset_flag(flag);
	}
	free(flag);
	if (is_tokens_true(cur))
		return (cur);
	return (0);
}

