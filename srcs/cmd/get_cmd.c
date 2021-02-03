#include "sh.h"
#include <stdio.h>

static char			*new_string(char *s)
{
	char		*n;
	size_t			i;

	i = ft_strlen(s) + 1;
	n = ft_strnew(i);
	return (n);
}

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

static void			get_env_val(char *buf, int *j, char *t, int *i, t_env **env)
{
	char			tmp[246];
	int				u;
	char			*t_tmp;
//	char			*f;

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
//	f = ft_strdup(tmp);
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
//	if (f)
	//	set_free_null(&f);
	if (t_tmp)
		set_free_null(&t_tmp);
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
	//buf = new_string(t->data);
	while (t->data[i])
	{
	//	while (t->data[i] != 34 && t->data[i] != 39 && t->data[i] != '$' && q[1] == 0)
		if (t->data[i] != 34 && t->data[i] != 39 && t->data[i] != '$' && q[1] == 0)
			buf[j++] = t->data[i++];
		if (t->data[i] == 39 && q[1] == 0)
		{
			q[0] = 1;
			i++;
			while (t->data[i] != 39)
			{
				buf[j] = t->data[i];
				j++;
				i++;
			}
			if(t->data[i] == 39)
			{
				q[0] = 0;
				i++;
			}
		}
		if (t->data[i] == 34 && q[0] == 0)
		{
			q[1] = 1;
			i++;
			while (t->data[i] && t->data[i] != 34)
			{
				if (t->data[i] == '$' && (t->data[i - 1] && t->data[i - 1] != 92))
					get_env_val(buf, &j, t->data, &i, env);
				buf[j] = t->data[i];
				j++;
				i++;
			}
			if (t->data[i] == 34)
			{
                q[1] = 0;
                i++;
            }
		}
		if (t->data[i] == '$' && t->data[i - 1] && t->data[i - 1] != 92 && q[0] == 0)
			get_env_val(buf, &j, t->data, &i, env);
	}
	buf[j] = '\0';
	c->arr = ft_strsplit(buf, ' ');
	//free(buf);
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
