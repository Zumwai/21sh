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

char                    **fill_res(char *s, int i, char **res)
{
    int l;

    l = ft_strlen(s) - i;
    res[0] = ft_strsub(s, 0, i);
    if (l != 0)
    	res[1] = ft_strsub(s, i + 1, l);
    else
    	res[1] = '\0'; /* what is the purpose? UB. res[1][0] to 0 or res[1] to NULL? */
    res[2] = '\0';
    return (res);
}

/*int						skobka(char *s, int i)
{
	char				buf;

	if (s[i] == '(')
		buf = ')';
	if (s[i] == '{')
		buf = '}';
	if (s[i] == '[')
		buf = ']';
	while (s[i] && s[i] != buf)
		i++;
	return (i);
}

int						count_args(char *s)
{
	int					res;
	int					i;

	i = 0;
	res = 1;
	while (s[i])
	{
		if (s[i] == ' ' && s[i + 1] && s[i + 1] != ' ')
			res++;
		if (s[i] == '{' || s[i] == '(' || s[i] == '[')
		{
			res++;
			i = skobka(s, i);
		}
		i++;
	}
	return (res);
}

char					*get_struct(char *s, int *i)
{

}

char					**save_the spaces(char *s)
{
	int					c;
	char				**res;
	int					i;
	int					j;

	i = 0;
	j = 0;
	c = count_argc(s);
	res = (char *)malloc(sizeof(char) * c + 1);
	while (i < c)
	{
		res[i] = get_struct(s, &j);
		i++;
	}
	res[i] = '\0';
	return (res);
}*/

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

char				**save_the_spaces(char *s)
{
	char    **res;
	int i;
	char    *cm;

	 i = 0;
	while (s[i] && s[i] != ' ' && s[i] != '\0')
	    i++;
	cm = ft_strsub(s, 0, i);
	if (ft_strcmp(cm, "echo") != 0 && ft_strcmp(cm, "awk") != 0)
	    res = ft_strsplit(s, ' ');
	else
    {
	    res = (char **)malloc(sizeof(char *) * 3);
	    fill_res(s, i, res);
    }
	free(cm);
    return (res);
}

static t_cmd			*get_data_cmd(t_token *t, t_cmd *c, t_env **env)
{
	int			i;
	int 		q[2]; /// 0 для одинарного, 1 для двойного
	char		buf[10000];
	int			j;

	i = 0;
	j = 0;
	q[0] = 0;
	q[1] = 0;
	while (t->data[i])
	{
		if (t->data[i] == 39 && q[1] == 0)
		{
			q[0] = q[0] == 0 ? 1 : 0;
			i++;
		}
		if (t->data[i] == 34 && q[0] == 0)
		{
			q[1] = q[1] == 0 ? 1 : 0;
			i++;
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
		if (t->data[i] != 92 && t->data[i] != 39 && t->data[i] != 34)
			buf[j++] = t->data[i++];
	}
	buf[j] = '\0';
	c->arr = save_the_spaces(buf); /// это надо, чтобы не потерять проебелы в начале строки в кавычках для echo a-la " hello "
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
