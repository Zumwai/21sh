#include "sh.h"

static char 				*get_data(char *line, int *n, t_flag *flag, t_token *t)
{
	char 			res[1000];
	int 			j;
	char			*ret;
	char			*buf;

	j = 0;
	while (line[*n] != '\0')
	{
		while (line[*n] != '|' && line[*n] != ';' /*&& line[*n] != '&' */ && line[*n] != '>' &&
			   line[*n] != '<' /*&& line[*n] != '\n' */&& line[*n] != '\0')
		{
			update_flag(flag, line[*n]);
			if (line[*n] == ' ' && t->prev && (ft_strcmp(t->prev->data, "<<") == 0))
			{
				res[j] = '\0';
				buf = ft_strdup(res);
				ret = ft_strtrim(buf);
				free(buf);
				return (ret);
			}
			if (line[*n] == '\n')
			{
				res[j] = '\0';
				*n += 1;
				buf = ft_strdup(res);
				ret = ft_strtrim(buf);
				free(buf);
				return (ret);
			}
			res[j] = line[*n];
			*n += 1;
			j += 1;
		}
		res[j] = line[*n];
		if (semantica(flag, line, n, &j) == 0)
			continue;
		if (semantica(flag, line, n, &j) == 1 && j != 0) {
			res[j] = '\0';
			buf = ft_strdup(res);
			ret = ft_strtrim(buf);
			free(buf);
			return (ret);
		}
		if (j == 0){
			ret = get_semantica_ret(line, n, res, j);
			*n += 1;
			return (ret);
		}
		else
		{
			res[j] = line[*n];
			*n += 1;
			j += 1;
		}
	}
	buf = ft_strdup(res);
	ret = ft_strtrim(buf);
	free(buf);
	return (ret);
}

int						is_empty(char *s)
{
	int					i;

	i = 0;
	while (s[i])
	{
		if (s[i] != ' ' && s[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

/*void print(t_token *cur)
{
	while (cur)
	{
		ft_putendl("new token  ");
		ft_putnbr(cur->c_type);
		ft_putendl(cur->data);
		cur = cur->next;
	}
}*/

extern t_token 			*parsing_t(char *line, t_env **env)
{
	t_token *token;
	int car;
	t_flag *flag;
	int l;
	t_token *cur;

	car = 0;
	if (!(l = ft_strlen(line)))
		return (0);
	if (is_empty(line))
		return (0);
	token = init_token();
	cur = token;
	flag = init_flag();
	token->data = get_data(line, &car, flag, token);
	while (car < l)
	{
		while (line[car] && line[car] == ' ')
			car++;
		if (line[car] != '\0')
		{
			token->next = init_token();
			token->next->prev = token;
			token = token->next;
			token->data = get_data(line, &car, flag, token);
		}
		//if (ft_strcmp(token->data, "") != 0)
			//token->data = get_data(line, &car, flag, token);
		flag = reset_flag(flag);
	}
	free(flag);
	if (is_tokens_true(cur, env))
		return (cur);
	return (0);
}

