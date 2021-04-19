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

static void			getter_val(char *buf, int *j, int *i, char *t_tmp) //.вынуть переменную среды
{
	int				u;

	u = 0;
	if (t_tmp)
	{
		while (t_tmp[u])
		{
			buf[*j] = t_tmp[u];
			*j = *j + 1;
			u++;
		}
	}
	*i = *i;
	if (t_tmp)
		set_free_null(&t_tmp);
}

char            get_litera(char *s, int *i, int *q)
{
    char res;

    if (*q == 0)
        *q = 1;
    else
        *q = 0;
   res = s[*i];
   *i = *i + 1;
    return (res);
}

char            get_litera_move_point(int *q, char *s, int *i)
{
    char        res;

    *i = *i + 1;
    if (q[1] == 0 && q[0] == 0)
        res = s[*i];
    else
        res = get_spec(s[*i]);
    *i = *i + 1;
    return (res);
}

char                    *env_for_eot(char *t, int *i, t_env **env)
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
    return (t_tmp);
}

char *get_str_for_eot(char *s, int *q, t_env **env)
{
    int			i;
    char		buf[10000];
    int			j;
    char        *tmp;

    i = 0;
    j = 0;
    while (s[i] != '\0')
    {
        if (s[i] == 39 && q[1] == 0)
            buf[j++] = get_litera(s, &i, &q[0]);
        if (s[i] == 34 && q[0] == 0)
            buf[j++] = get_litera(s, &i, &q[1]);
        if (s[i] == 92 && s[i + 1])
            buf[j++] = get_litera_move_point(q, s, &i);
        if (s[i] == '$' && q[0] == 0 && s[i + 1] && s[i + 1] != '$' && s[i + 1] != ' ')
        {
            tmp = env_for_eot(s, &i, env);
            getter_val(buf, &j, &i, tmp);
        }
        if (s[i] && s[i] != 92)
            buf[j++] = s[i++];
    }
    buf[j] = '\0';
    return (ft_strdup(buf));
}

char			*str_todoc(char *s, int *qu, t_env **env)
{
	int 		q[2]; /// 0 для одинарного, 1 для двойного
	char		*src;

	q[0] = 0;
	q[1] = 0;
	if (*qu > 0)
	{
		if (*qu == 1)
			q[0] = 1;
		else
			q[1] = 1;
	}
	src = get_str_for_eot(s, q, env);
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

extern t_token 			*get_last_token(t_token *t)
{
	t_token 		*r;

	r = t;
	while (r->next != NULL)
		r = r->next;
	return (r);
}

void                    settle_all(t_token *fix, char *name, int fd)
{
    set_free_null(&fix->data);
    free(fix);
    free(name);
    close(fd);
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
	   if (t->c_type == 9 || t->next->c_type == 9 || t->prev->c_type == 9
	    || t->type == 1 || t->prev->type == 1)
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

	tar = NULL;
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

void                    make_command_state(t_token *t)
{
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

extern int 				is_tokens_true(t_token *s, t_env **env)
{
	t_token				*t;

	t = s;
	make_command_state(t);
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
}

extern t_token 			*init_token(void)
{
	t_token		*new;

	if(!(new = (t_token *)malloc(sizeof(t_token))))
		return (NULL);
	ft_memset(new, 0, sizeof(t_token));
	return (new);
}