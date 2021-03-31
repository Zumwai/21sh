#include "sh.h"
#include <stdio.h>

static char				*get_value(char *name, t_env **env)
{
	t_env			*cur;
	char			*res;

	cur = NULL;
	res = NULL;
	cur = find_env_variable(env, name);
	if (cur)
		res = ft_strdup(cur->value);
	return res;
}

static void			get_env_val(char *buf, int *j, char *t, int *i, t_env **env) //.вынуть переменную среды
{
	char			tmp[246];
	int				u;
	char			*t_tmp;

	u = 0;
	*i = *i + 1;
	while (t[*i] != ' ' && t[*i] != '/' && t[*i] != 92
	&& t[*i] != 34 && t[*i] !=39 && t[*i])
	{
		tmp[u] = t[*i];
		*i = *i + 1;
		u++;
	}
	tmp[u] = '\0';
	if (t[*i] == '"')
	    *i = *i + 1;
	t_tmp = get_value(tmp, env);
	if (t_tmp)
	{
		u = 0;
		while (t_tmp[u])
		{
			buf[*j] = t_tmp[u];
			*j = *j + 1;
			u++;
		}
	}
	*i = *i - 1;
	if (t_tmp)
		set_free_null(&t_tmp);
}

int						len_of_word(char *s, int i)
{
	int					res;
	char				c;


	res = 0;
	c = 0;
	while (s[i] && s[i] != ' ')
	{
		if (s[i] == c)
			c = 0;
		else if (s[i] != ' ' && s[i] != 34 && s[i] != 39)
			res++;
		else if (s[i] == '"' || s[i] == '\'')
		{
			c = s[i];
			i++;
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

char					*fill_str(char *s, int *i)
{
	char				*res = NULL;
	int					j = 0;
	int					l = 0;
	char				buf[1000] = {0};
	int 				course = 0;
	char 				c = 0;

	j = 0;
	course = 0;
	course = (*i);

	ft_strclr(buf);
	if (s[course] == ' ')
	{
		while (s[course] == ' ' && s[course])
			course++;
	}
	l = len_of_word(s, course);
	res = NULL;
	while (j < l && s[course])
	{
		if (s[course] == c)
			c = 0;
		else if (c == 0 && (s[course] == '"' || s[course] == 39))
		{
			c = s[course];
		}
		else if (s[course] != c) {
			buf[j] = s[course];
			j++;
		}
		course = course + 1;
	}
	buf[j] = '\0';
	///if (its_redir(buf))
	///fill_str(s, &i);
	///else
	res = ft_strdup(buf);
	(*i) = course;
	return(res);
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

int					how_much_words(char *s)
{
	int					i;
	int					res;
	char				c;

	i = 0;
	res = 0;
	c = 0;
	while (s[i])
	{
		if (s[i] == 34 || s[i] == 39)
		{
			c = s[i];
			i++;
			while (s[i] && s[i] != c)
			{
				i++;
			}
			i = i + 1;
		}
		if (s[i] != '\0' && s[i] != ' ')
		{
			while (s[i] != '\0' && s[i] != ' ')
				i++;
		}
		if (s[i] == ' ')
		{
			while (s[i] == ' ')
				i++;
		}
		res++;
	}
	///printf("how_much_words === %d\n", res);
	return (res);
}

char					**s_to_arr(char *s)
{
	int					i;
	char				**res;
	int					j;
	int					c;
	int 				f;
	int					g;

	c = 0;
	j = 0;
	f = how_much_words(s);
	g = how_much_restreams(s);
	i = f - g;
	res = (char **)malloc(sizeof(char *) * (i + 1));
	int tmp = 0;
	while (tmp < i)
	{
		res[tmp] = NULL;
		tmp++;
	}
	while (j < i)
	{
		res[j] = fill_str(s, &c);
		j++;
		if (s[c] != 0)
			c = c + 1;
	}
	res[j] = NULL;
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

static t_cmd			*get_data_cmd(t_token *t, t_cmd *c, t_env **env)
{
	int			i;
	int 		q[2]; /// 0 для одинарного, 1 для двойного
	char		buf[10000];
	int			j;
	int			d;
	char		*src;

	i = 0;
	j = 0;
	q[0] = 0;
	q[1] = 0;

	while (t->data[i])
	{
		if (t->data[i] == 39 && q[1] == 0)
		{
			q[0] = q[0] == 0 ? 1 : 0;
			buf[j++] = t->data[i++];
		}
		if (t->data[i] == 34 && q[0] == 0)
		{
			q[1] = q[1] == 0 ? 1 : 0;
			buf[j++] = t->data[i++];
		}
		if (t->data[i] == 92 && t->data[i + 1])
		{
			if (q[1] == 0 && q[0] == 0)
			{
				i++;
				buf[j++] = t->data[i++];
			}
			else
			{
				i++;
				buf[j++] = get_spec(t->data[i]);
				i++;
			}
		}
		if (t->data[i] == '$' && q[0] == 0 && t->data[i + 1] && t->data[i + 1] != '$' && t->data[i + 1] != ' ')
		{
			get_env_val(buf, &j, t->data, &i, env);
			i++;
		}
		if (t->data[i] && t->data[i] != 92)
			buf[j++] = t->data[i++];
	}
	buf[j] = '\0';
	src = ft_strdup(buf);
	c->arr = s_to_arr(src);
	free(src);
	return (c);
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
			cur->type = cur_t->next->c_type;
			cur_t = cur_t->next->next;
			cur->next = new_cmd(cur);
			cur = cur->next;
		}
		else
			break ;
	}

	return (head);
}
