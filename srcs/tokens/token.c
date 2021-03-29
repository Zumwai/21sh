#include "sh.h"

static char				*ge_value(char *name, t_env **env)
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

static void			getter_val(char *buf, int *j, char *t, int *i, t_env **env) //.вынуть переменную среды
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
	t_tmp = ge_value(tmp, env);
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

char			*str_todoc(char *s, int *qu, t_env **env)
{
	int			i;
	int 		q[2]; /// 0 для одинарного, 1 для двойного
	char		buf[10000];
	int			j;
	char		*src;

	q[0] = 0;
	q[1] = 0;
	i = 0;
	j = 0;
	ft_putnbr(*qu);
	ft_putendl("=== quots");
	ft_putstr(s);
	ft_putendl(" === src todoc");
	if (*qu > 0)
	{
		if (*qu == 1)
			q[0] = 1;
		else
			q[1] = 1;
	}
	while (s[i] != '\0')
	{
		ft_putendl("in while");
		if (s[i] == 39 && q[1] == 0)
		{
			q[0] = q[0] == 0 ? 1 : 0;
			buf[j++] = s[i++];
		}
		if (s[i] == 34 && q[0] == 0)
		{
			q[1] = q[1] == 0 ? 1 : 0;
			buf[j++] = s[i++];
		}
		if (s[i] == 92 && s[i + 1])
		{
			if (q[1] == 0 && q[0] == 0)
			{
				i++;
				buf[j++] = s[i++];
			}
			else
			{
				i++;
				buf[j++] = get_spec(s[i]);
				i++;
			}
		}
		if (s[i] == '$' && q[0] == 0 && s[i + 1] && s[i + 1] != '$' && s[i + 1] != ' ')
		{
			getter_val(buf, &j, s, &i, env);
			i++;
		}
		if (s[i] && s[i] != 92)
			buf[j++] = s[i++];
	}
	buf[j] = '\0';
	src = ft_strdup(buf);
	ft_putstr(src);
	ft_putendl(" === res of todoc");
	return (src);
}

char					*exec_name(char *s, int *q)
{
	char				*res;
	int					i;
	int					l;
	char				c;
	int					j;

	ft_putstr("exec_name arg ");
	ft_putendl(s);
	i = 0;
	l = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
		{
			c = s[i];
			i++;
			while (s[i] != c || s[i])
			{
				l++;
				i++;
			}
		}
		i++;
		l++;
	}
	ft_putnbr(l);
	ft_putendl("=== l");
	res = (char *)malloc(sizeof(char) * l + 1);
	i = 0;
	j = 0;
	while (s[i]) {
		if (s[i] != '"' && s[i] != '\'')
		{
			res[j] = s[i];
			j++;
			i++;
	}
		if (s[i] == '"' || s[i] == '\'')
		{
			c = s[i];
			*q = c == '"' ? 2 : 1;
			i++;
			while (s[i] != c || s[i])
			{
				res[j] = s[i];
				j++;
				i++;
			}
		}
	}
	res[l] = '\0';
	ft_putendl(res);
	ft_putendl("=== nme");
	return (res);
}

extern t_token 			*get_last_token(t_token *t)
{
	t_token 		*r;

	r = t;
	while (r->next != NULL)
		r = r->next;
	return (r);
}

void					*make_doc(char *tar, t_token *t, t_env **env)
{
	char *name;
	int fd;
	int		l;
	t_token		*fix;
	int			q;
	char		*todoc;

	q = 0;
	name = exec_name(tar, &q);
	ft_putstr(name);
	ft_putendl("   file_name");
	fd = open(name, O_CREAT | O_RDWR | O_APPEND,
			  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	while (ft_strcmp(name, t->data) != 0)
	{
		if (t->c_type != 9 && t->prev && t->prev->c_type != 9)
		{
			todoc = str_todoc(t->data, &q, env);
			l = ft_strlen(todoc);
			write(fd, todoc, l);
			///fix = t;
			///t->prev->next = fix->next;
			t = t->next;
			///ft_strclr(fix->data);
			///free(fix);
		}
		else
			t = t->next;
	}
	close(fd);
}

static int				right_row(t_token *t, t_env **env)
{
	///t_token 		*first;
	char			*tar;

	ft_putendl("right_row");
	///first = t;
	if (t->type == 1)
		return (0);
	if (t->next)
	{
		ft_putendl("wtf?! -- 1");
		while (t->next)
		{
			if (t->c_type == 9)
			{
				ft_putendl("wtf?!");
				t = t->next;
				tar = t->data;
				///tar = exec_name(first->data);
				make_doc(tar, t->next, env);
			}
			//if (first->type == first->next->type)
			//return (0);
			t = t->next;
		}
	}
	ft_putendl("right_row done");
	return (1);
}

static int 				get_priority(char *s)
{
	//if (ft_strcmp(s, BK) == 0)
	//return ()
	if (ft_strcmp(s, OR) == 0)
		return (2);
	if (ft_strcmp(s, AND) == 0)
		return (2);
	if (ft_strcmp(s, PIPE) == 0)
		return (3);
	if (ft_strcmp(s, SC) == 0)
		return (1);
	/* Error */
	return 0;
}

static int					get_cmd_type(char *s)
{
	if (ft_strcmp(s, BK) == 0)
		return (bk);
	if (ft_strcmp(s, OR) == 0)
		return (ro);
	if (ft_strcmp(s, AND) == 0)
		return (and);
	if (ft_strcmp(s, PIPE) == 0)
		return (pi);
	if (ft_strcmp(s, SC) == 0)
		return (sc);
	if (ft_strcmp(s, GREAT) == 0)
		return (out_w);
	if (ft_strcmp(s, GGREAT) == 0)
		return (out_add);
	if(ft_strcmp(s, LESS) == 0)
	    return (in);
	if(ft_strcmp(s, LLESS) == 0)
		return (heredoc);
	return 0;
}

extern int 				is_tokens_true(t_token *s, t_env **env)
{
	t_token				*t;

	t = s;
	ft_putendl("is_tokens_true");
	while (t)
	{
		if (ft_strcmp(t->data, BK) == 0 || ft_strcmp(t->data, SC) == 0 ||
				ft_strcmp(t->data, OR) == 0 ||
				ft_strcmp(t->data, AND) == 0 ||
				ft_strcmp(t->data, PIPE) == 0 ||
				ft_strcmp(t->data, GREAT) == 0 ||
				ft_strcmp(t->data, GGREAT) == 0 ||
				ft_strcmp(t->data, LESS) == 0 ||
				ft_strcmp(t->data, LLESS) == 0 )
		{
			t->type = 1;
			t->c_type = get_cmd_type(t->data);
			t->priority = get_priority(t->data);
		}
		else
			t->type = 0;
		t = t->next;
	}
	t = s;
	if (right_row(t, env))
		return (1);
	//else
	    //ft_putendl_fd("error of parsing", 2);
	//return (0);

}

extern t_token 			*init_token(void)
{
	t_token		*new;

	if(!(new = (t_token *)malloc(sizeof(t_token))))
		return (NULL);
	ft_memset(new, 0, sizeof(t_token));
	return (new);
}