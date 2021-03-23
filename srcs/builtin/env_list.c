#include "sh.h"
/*
static char			*get_env_name(char *s)
{
	char		*nm;
	int			l;
	int			i;

	i = 0;
	l = 1;
	while (s[l] != '=')
		l++;
	nm = (char *)malloc(sizeof(char) * l + 1);
	while (s[i] != '=')
	{
		nm[i] = s[i];
		i++;
	}
	nm[i] = '\0';
	return (nm);
}

static int				arr_len(char **env)
{
	int			len;
	int			i;

	len = -1;
	i = 0;
	while (env[i])
	{
		i++;
		len++;
	}
	return (len);
}

static char			*get_env_val(char *env, char *value)
{
	int			i;
	int			c;

	i = 0;
	c = 0;
	while (env[i] != '=')
		i++;
	value = (char *)malloc(sizeof(char) * ft_strlen(env) - i);
	i++;
	while (env[i])
		value[c++] = env[i++];
	value[c] = '\0';
	return (value);
}

static t_env				*make_new_list(void)
{
	t_env			*new;

	new = (t_env *)malloc(sizeof(t_env));
	new->next = NULL;
	return (new);
}

static void			get_env(t_env *env)
{
	extern char	**environ;
	int			i;
	t_env		*cur;
	int			len;

	len = arr_len(environ);
	i = 0;
	cur = env;
	while (environ[i])
	{
		cur->name = get_env_name(environ[i]);
		cur->value = get_env_val(environ[i], cur->value);
		if (len)
		{
			cur->next = make_new_list();
			cur = cur->next;
		}
		if (!len)
			break ;
		i++;
		len--;
	}
}

extern t_env			*env_list(void)
{
	t_env		*env;

	env = make_new_list();
	get_env(env);
	return (env);
}
*/
