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

	size = 0;
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

int						len_of_word(char *s, int i)
{
	int					res;
	char				c;


	res = 0;
	c = 0;
	printf("len s == %s i == %d\n", s, i);
	while (s[i] && s[i] != ' ')
	{
		if (s[i] == c)
			c = 0;
		else if (s[i] != ' ' && s[i] != 34 && s[i] != 39)
		{
			printf("s[%d] === %c\n", i, s[i]);
			res++;
		}
		else if (s[i] == '"' || s[i] == '\'')
		{
			c = s[i];
			i++;
			printf("c === %c\n", c);
			while (s[i] != c && s[i]) {
				printf("sss[%d] === %c\n", i, s[i]);
				i++;
				res++;
			}
		}
		else if (s[i] == ' ' && !c)
		{
			break;
		}
		i++;
	}
	printf("len_of_word === %d\n", res);
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
	char				*buf;
	int 				course = 0;
	char 				c = 0;

	j = 0;
	course = 0;
	course = (*i);

	//ft_putendl("BEWARE");
	buf = ft_strdup(s);
	//printf("start course === %d\n", course);
	//printf("st.course === %c\n", s[course]);
	if (s[course] == ' ')
	{
		while (s[course] == ' ' && s[course])
			course++;
	}
	printf("s === %s\n", s);
	///printf("course === %d\n", course);
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
	///*i = *i + 1;
	buf[j] = '\0';
	///if (its_redir(buf))
	///fill_str(s, &i);
	///else
	printf("res == %s\n", buf);
	(*i) = course;
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
	printf("how_much_words === %d\n", res);
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
	printf("%d === words\n", i);
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
		///printf("s_to_arr c === %d\n", c);
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
	char		*buf;
	int			j;
	int			d;
	char		*src;
	int			size;
	i = 0;
	j = 0;
	q[0] = 0;
	q[1] = 0;

	size = ft_strlen(t->data);
	buf = ft_strnew(size);
	while (t->data[i])
	{
		if (t->data[i] == 39 && q[1] == 0)
		{
			q[0] = (q[0] == 0) ? 1 : 0;
			buf[j++] = t->data[i];
		}
		else if (t->data[i] == 34 && q[0] == 0)
		{
			q[1] = (q[1] == 0) ? 1 : 0;
			buf[j++] = t->data[i];
		}
		else if (t->data[i] == 92 && t->data[i + 1])
		{
			if (q[1] == 0 && q[0] == 0)
			{
				i++;
				buf[j++] = t->data[i];
			}
			else
			{
				i++;
				buf[j++] = get_spec(t->data[i]);
			}
		}
		else if (t->data[i] == '$' && q[0] == 0 && t->data[i + 1] && t->data[i + 1] != '$' && t->data[i + 1] != ' ')
		{
			buf = insert_env_val(buf, &j, t->data, &i, env, &size);
		}
		else if (t->data[i] && t->data[i] != 92)
			buf[j++] = t->data[i];
		i++;
	}
	buf[j] = '\0';
	src = ft_strdup(buf);
	printf("src === %s\n", src);
	c->arr = s_to_arr(src);
	free(src);
	free(buf);
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
