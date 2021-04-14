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
	if (*qu > 0)
	{
		if (*qu == 1)
			q[0] = 1;
		else
			q[1] = 1;
	}
	while (s[i] != '\0')
	{
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
	return (src);
}

char					*exec_name(char *s, int *q)
{
    char buf[256];
    char c;
    int i;
    int j;
    char *res;

    j = 0;
    i = 0;
    while (s[i])
    {
        if (s[i] == '"' || s[i] == '\'')
        {
            c = s[i++];
            *q = c == '"' ? 2 : 1;
            while (s[i] != c)
                buf[j++] = s[i++];
            if (s[i] == c)
                i++;
        }
        else
            buf[j++] = s[i++];
    }
    buf[j] = '\0';
    res = ft_strdup(buf);
    return (res);
}
	/*char				*res;
	int					i;
	int					l;
	char				c;
	int					j;

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
	ft_putstr("exec_name == ");
	ft_putendl(res);
	return (res);
}*/

extern t_token 			*get_last_token(t_token *t)
{
	t_token 		*r;

	r = t;
	while (r->next != NULL)
		r = r->next;
	return (r);
}

void					make_doc(char *tar, t_token *t, t_env **env)
{
	char *name;
	int fd;
	t_token *prev;
	t_token		*fix;
	int			q;
	char		*todoc;

	q = 0;
	name = exec_name(tar, &q);
	fd = open(name, O_CREAT | O_RDWR | O_APPEND,
			  S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	while (ft_strcmp(name, t->data) != 0)
	{
	    if (t->c_type == 9 || t->next->c_type == 9 || t->prev->c_type == 9 || t->type == 1 || t->prev->type == 1)
        {
	        t = t->next;
	        continue ;
        }
			todoc = str_todoc(t->data, &q, env);
			ft_putendl_fd(todoc, fd);
			prev = t->prev;
			fix = t;
			t = t->next;
			prev->next = t;
			t->prev = prev;
			set_free_null(&fix->data);
			free(fix);
			set_free_null(&todoc);
	}
	prev = t->prev;
	fix = t;
	t = t->next;
	prev->next = t;
	if (t)
		t->prev = prev;
	set_free_null(&fix->data);
	free(fix);
	free(name);
	close(fd);
}

static int				right_row(t_token *t, t_env **env)
{
	char			*tar;

	if (t->type == 1)
		return (0);
	while (t)
	{
	    if (t->c_type == 9)
	    {
	        t = t->next;
	        tar = t->data;
	        make_doc(tar, t->next, env);
	    }
	    //if (first->type == first->next->type)
	    //return (0);
	    t = t->next;
	}
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

int                     is_it_command(t_token *t)
{
    if (ft_strcmp(t->data, BK) == 0 || ft_strcmp(t->data, SC) == 0 ||
     ft_strcmp(t->data, OR) == 0 ||
     ft_strcmp(t->data, AND) == 0 ||
     ft_strcmp(t->data, PIPE) == 0 ||
     ft_strcmp(t->data, GREAT) == 0 ||
     ft_strcmp(t->data, GGREAT) == 0 ||
     ft_strcmp(t->data, LESS) == 0 ||
     ft_strcmp(t->data, LLESS) == 0 )
        return (1);
    else
        return (0);
}

extern int 				is_tokens_true(t_token *s, t_env **env)
{
	t_token				*t;

	t = s;
	while (t)
	{
	    if (is_it_command(t))
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
    {
        t = s;
        while (t)
        {
            if (is_it_command(t))
            {
                t->type = 1;
                t->c_type = get_cmd_type(t->data);
                t->priority = get_priority(t->data);
            }
            else
                t->type = 0;
            t = t->next;
        }
    }
		return (1);
	//else
	    //ft_putendl_fd("error of parsing", 2);
	return (0);
}

extern t_token 			*init_token(void)
{
	t_token		*new;

	if(!(new = (t_token *)malloc(sizeof(t_token))))
		return (NULL);
	ft_memset(new, 0, sizeof(t_token));
	return (new);
}