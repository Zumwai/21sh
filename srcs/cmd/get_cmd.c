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
	if (s[i] == '"' || s[i] == 39)
	{
		c = s[i];
		i = i + 1;
		while (s[i] && s[i] != c)
		{
			res++;
			i = i + 1;
		}
	}
	else
	{
		while (s[i] && s[i] != ' ')
		{
			res++;
			i = i + 1;
		}
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
	char				*res;
	int					j;
	int					l;
	char				buf[1000];
	int 				course;


	j = 0;
	if (s[*i] == ' ')
	{
		while (s[*i] && s[*i] == ' ')
			*i = *i + 1;
	}
	course = (*i);
	l = len_of_word(s, course);
	res = NULL;
	while (j < l)
	{
		if (s[*i] == '"' || s[*i] == 39)
				*i = *i + 1;
		buf[j] = s[*i];
		j++;
		*i = *i + 1;
	}
	buf[j] = '\0';
	if (its_redir(buf))
		fill_str(s, &i);
	else
		res = ft_strdup(buf);
	//printf("res === %s\n", res);
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
 	//printf("start === %d", res);
 	//printf("s === %s\n", s);
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
 			i++;
			res = res + 1;
		}
 		if (s[i] && s[i] != ' ' && (s[i + 1] == ' ' || s[i + 1] == '\0' || s[i] == 34 || s[i] == 39))
		{
 			//printf("here mistake?\n");
			res = res + 1;
		}
 		i++;
	}
 	//printf("how much words === %d\n", res);
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
 	res = (char **)malloc(sizeof(char *) * i + 1);
 	while (j < f)
	{
 		res[j] = fill_str(s, &c);
 		j++;
 		c++;
	}
 	res[j] = '\0';
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
		if (t->data[i] && t->data[i] != 92 && t->data[i] != 39 && t->data[i] != 34)
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
