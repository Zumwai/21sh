#include "sh.h"
#include <stdio.h>

static char				*get_value(char *name, t_env **env)
{
	t_env			*cur;
	char			*res;

	cur = NULL;
	res = NULL;
	cur = find_env_variable(env, name);
	if (cur) {
		if (cur->value)
			res = ft_strdup(cur->value);
	}
	return res;
}

static char		*insert_env_val(char *buf, int *j, char *orig, int *i, t_env **env, int *o_size)
{
	int		size;
	char	*var;
	char	*value;
	char	*buffer;

	size = 1;
	var = NULL;
	value = NULL;
	buffer = NULL;
	while (orig[*i + size] != ' ' && orig[*i + size] != '/' && orig[*i + size] != '\\'
	&& orig[*i + size] != '\"' && orig[*i + size]  != '\'' && orig[*i + size]
	&& orig[*i + size] != ';' && orig[*i + size + 1] != '$')
		size++;
	if (orig[*i + size] && orig[*i + size + 1] == '$')
		size++;
	if (size)
		var = ft_strndup(&orig[*i], size);
	if (var)
		value = get_value(&var[1], env);
	if (value)
	{
		*i = *i + size - 1;
		size = ft_strlen(value);
		buffer = ft_strnew(*o_size + size);
		ft_strcpy(buffer, buf);
		ft_strcat(buffer, value);
		*o_size = *o_size + size;
		*j = *j + size;
		free(buf);
		buf = buffer;
		free(value);
	}
	else
		*i = *i + 1;
	free(var);
	return (buf);
}

int                     get_res_for_len(char *s, int i, char c, int res)
{
    while (s[i] && s[i] != ' ')
    {
        if (s[i] == c)
            c = 0;
        else if (s[i] != ' ' && s[i] != 34 && s[i] != 39)
            res++;
        else if (s[i] == '"' || s[i] == '\'')
        {
            c = s[i++];
            while (s[i] != c && s[i])
            {
                i++;
                res++;
            }
        }
        else if (s[i] == ' ' && !c)
            break;
        i++;
    }
    return (res);
}

int						len_of_word(char *s, int i)
{
	int					res;
	char				c;

	res = 0;
	c = 0;
	res = get_res_for_len(s, i, c, res);
	return (res);
}

int						its_redir(char *s)
{
	int					i;

	i = 0;
	if (s[i] == '"' || s[i] == '\'')
		return (0);
	while (s[i])
	{
		if (s[i] == '>' && s[i] && s[i + 1] == '&')
            return (1);
		if (s[i] == '<' && s[i] && s[i + 1] == '&')
			return (1);
		if (s[i] == '>' && s[i - 1] && s[i - 1] == '&')
			return (1);
		i++;
	}
	return (0);
}

t_cmd                   *fill_aggr(char *s, t_cmd *c)
{
    if (c->fd1 != 0)
        return (c);
    if (s[0] == '1' || s[0] == '2')
        c->fd1 = s[0] - '0';
    if (s[3] == '1' || s[3] == '2')
        c->fd2 = s[3] - '0';
    if (s[3] == '-')
        c->fd2 = -1;
    return (c);
}

int                     get_course(char *s, int course)
{
    while (s[course] == ' ' && s[course])
        course++;
    return (course);
}

char					*fill_str(char *s, int *i, t_cmd *cmd)
{
	char				*res;
	int					j;
	int					l;
	char				*buf;
	int 				course;
	char 				c;

	c = 0;
	j = 0;
	course = (*i);
	buf = ft_strdup(s);
	if (s[course] == ' ')
	    course = get_course(s, course);
	l = len_of_word(s, course);
	res = NULL;
	while (j < l && s[course])
	{
		if (s[course] == c)
			c = 0;
		else if (c == 0 && (s[course] == '"' || s[course] == 39))
			c = s[course];
		else if (s[course] != c) {
			buf[j] = s[course];
			j++;
		}
		course = course + 1;
	}
	buf[j] = '\0';
    (*i) = course;
    if (its_redir(buf))
    {
	    cmd = fill_aggr(buf, cmd);
	    free(buf);
	    if (s[*i])
	        fill_str(s, &i, cmd);
	    else
	        return (NULL);
    }
	return(buf);
}

int						how_much_restreams(char *s)
{
	int					i;
	int					res;

	i = 0;
	res = 0;
	while (s[i])
	{
		if (s[i] == '>' && s[i] && s[i + 1] == '&')
			res++;
		if (s[i] == '<' && s[i] && s[i + 1] == '&')
			res++;
		if (s[i] == '>' && s[i - 1] && s[i - 1] == '&')

			res++;
		i++;
	}
	return (res);
}

int                 moving_str_pointer(char *s, int i)
{
    char            c;

    c = 0;
    if (s[i] == 34 || s[i] == 39)
    {
        c = s[i];
        i++;
        while (s[i] && s[i] != c)
            i++;
        i = i + 1;
    }
    if (s[i] != '\0' && s[i] != ' ')
    {
        while (s[i] != '\0' && s[i] != ' ')
            i++;
    }
    if (s[i] == ' ')
        while (s[i] == ' ')
            i++;
    return (i);
}

int					how_much_words(char *s)
{
	int					i;
	int					res;

	i = 0;
	res = 0;
	while (s[i])
	{
	    i = moving_str_pointer(s, i);
		res++;
	}
	return (res);
}

char					**s_to_arr(char *s, t_cmd *cmd)
{
	int					i;
	char				**res;
	int					j;
	int					c;

	c = 0;
	j = 0;
	i = how_much_words(s) - how_much_restreams(s);
	res = (char **)malloc(sizeof(char *) * (i + 1));
	int tmp = 0;
	while (tmp < i)
		res[tmp++] = NULL;
	while (j < i)
	{
		res[j++] = fill_str(s, &c, cmd);
		if (s[c] != 0)
			c = c + 1;
	}
	if (s[c])
	    res[j] = fill_str(s, &c, cmd);
	res[j] = NULL;
	free(s);
	return (res);
}

char					get_spec(char s)
{
	char				res;

	res = s;
	if (s == 'n')
		res = '\n';
	if (s == 't')
		res = '\t';
	if (s == 'r')
		res = '\r';
	if (s == 'v')
		res = '\v';
	if (s == 'f')
		res = '\f';
	return (res);
}

int                     get_q(int q)
{
    if (q == 0)
        return (1);
    return (0);
}

char                    get_char(int *q, char *t, int i)
{
    char b;

    if (t[i] == 39 && q[1] == 0)
    {
        q[0] = get_q(q[0]);
        b = t[i];
    }
    else if (t[i] == 34 && q[0] == 0)
    {
        q[1] = get_q(q[1]);
        b = t[i];
    }
    else if (t[i] == 92 && t[i + 1])
    {
        i++;
        if (q[1] == 0 && q[0] == 0)
            b = t[i];
        else
            b = get_spec(t[i]);
    }
    else if (t[i] && t[i] != 92)
        b = t[i];
    return (b);
}

static t_cmd			*get_data_cmd(t_token *t, t_cmd *c, t_env **env)
{
	int			i;
	int 		q[2]; /// 0 для одинарного, 1 для двойного
	char		*buf;
	int			j;
	int			size;

	i = 0;
	j = 0;
	q[0] = 0;
	q[1] = 0;
	size = ft_strlen(t->data);
	buf = ft_strnew(size);
	while (t->data[i])
	{
		if ((t->data[i] == 39 && q[1] == 0) || (t->data[i] == 34 && q[0] == 0)
		|| (t->data[i] == 92 && t->data[i + 1]) || (t->data[i] && t->data[i] != 92))
		    buf[j++] = get_char(q, t->data, i);
		else if (t->data[i] == '$' && q[0] == 0 && t->data[i + 1] && t->data[i + 1] != ' ')
			buf = insert_env_val(buf, &j, t->data, &i, env, &size);
		i++;
	}
	buf[j] = '\0';
	c->arr = s_to_arr(buf, c);
	return (c);
}

t_cmd           *get_next_cmd(t_cmd *cmd, t_token *token)
{
    cmd->type = token->next->c_type;
    cmd->next = new_cmd(cmd);
    cmd = cmd->next;
    return (cmd);
}


t_cmd			*get_cmd(t_token *t, t_env **env)
{
	t_token		*cur_t;
	t_cmd		*head;
	t_cmd		*cur;

	cur_t = t;
	head = init_cmd();
	cur = head;
	if (!head)
		return NULL;
	while (cur_t)
    {
		cur = get_data_cmd(cur_t, cur, env);
		if (cur_t->next && cur_t->next->next)
		{
			cur = get_next_cmd(cur, cur_t);
			cur_t = cur_t->next->next;
		}
		else
			break ;
	}
	return (head);
}
