#include "sh.h"

char                        *eot_str(char *res, int j, int *n)
{
    char                       *buf;
    char                        *ret;

    *n += 1;
    buf = ft_strdup(res);
    ret = ft_strtrim(buf);
    set_free_null(&buf);
    return (ret);
}

int                         not_command(char c)
{
    if (c != '|' && c != ';' && c != '&' && c != '>' &&
    c != '<' && c != '\0')
        return (1);
    return (0);
}

void                        standart_step(char *res, char *line, char *n, char *j)
{
    res[*j] = line[*n];
    *n += 1;
    *j += 1;
}

char                        *its_ret(char *line, int *n, char *res, int j)
{
    char                    *ret;

    if (j == 0)
        ret = get_semantica_ret(line, n, res, j);
    else
    {
        res[j] = '\0';
        ret = ft_strtrim(res);
    }
    return (ret);
}

char                        *go_for_ret(char *line, int j, char *res, int *n)
{
    char *ret;

    res[j] = '\0';
    if (line[*n] == '\n')
        ret = eot_str(res, j, n);
    if (line[*n] == ' ')
        ret = ft_strtrim(res);
    return (ret);
}

static char 				*get_data(char *line, int *n, t_flag *flag, t_token *t)
{
	char 			*res;
	int 			j;
	char			*ret;

	j = 0;
	ret = NULL;
	res = ft_strdup(line);
	while (line[*n] != '\0')
	{
		while(not_command(line[*n]))
        {
			update_flag(flag, line[*n]);
			if ((line[*n] == ' ' && t->prev && (ft_strcmp(t->prev->data, "<<") == 0)) ||
			(line[*n] == '\n' && ((flag->u_quot % 2 == 0) && (flag->d_quot % 2 == 0))))
            {
			    ret = go_for_ret(line, j, res, n);
			    break ;
            }
			standart_step(res, line, n, &j);
		}
		res[j] = line[*n];
		if ((semantica(flag, line, n, &j) == 1 && j != 0) || j == 0)
		{
            ret = its_ret(line, n, res, j);
            break ;
        }
        standart_step(res, line, n, &j);
	}
	if (!ret)
		ret = ft_strtrim(res);
    set_free_null(&res);
	return (ret);
}

int						is_empty(char *s)
{
	int					i;

	i = 0;
	while (s[i])
	{
		if (s[i] != ' ' && s[i] != '\t')
			return (1);
		i++;
	}
	return (0);
}

int                     check_line(char *s)
{
    if (!ft_strlen(s) || !is_empty(s))
        return (0);
    return (1);
}

t_token                 *making_tokens(char *line, t_token *t, t_flag *flag)
{
    int                 car;
    int                 l;
    t_token             *head;

    car = 0;
    head = t;
    l = ft_strlen(line);
    t->data = get_data(line, &car, flag, t);
    while (car < l)
    {
        while (line[car] && line[car] == ' ')
            car++;
        if (line[car] != '\0')
        {
            t->next = init_token();
            t->next->prev = t;
            t = t->next;
            t->data = get_data(line, &car, flag, t);
        }
        flag = reset_flag(flag);
    }
    return (head);
}

extern t_token 			*parsing_t(char *line, t_env **env)
{
	t_token *token;
	t_flag *flag;

	if (check_line(line) == 0)
	    return (0);
	token = init_token();
	flag = init_flag();
	token = making_tokens(line, token, flag);
	free(flag);
	if (is_tokens_true(token, env))
		return (token);
	return (0);
}

