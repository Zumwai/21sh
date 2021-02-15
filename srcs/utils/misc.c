#include "sh.h"

int ft_abs(int i)
{
	return ((i > 0) ? i : -i);
}

t_env	*find_env_variable(t_env **env, char *sought)
{
	t_env *curs;

	curs = *env;
	while (curs)
	{
		if (!ft_strcmp(curs->name, sought))
			return curs;
		curs = curs->next;
	}
	return NULL;
}

void	ft_concat(char *str, char **path, char *name)
{
	ft_strclr((*path));
	(*path) = strcpy((*path), str);
	(*path) = strcat((*path), "/");
	if (name)
		(*path) = strcat((*path), name);
}
void	ft_strclr(char *s)
{
	size_t		i;

	if (!s)
		return ;
	i = 0;
	while (s[i] != '\0')
	{
		s[i] = '\0';
		i++;
	}
}
char	*get_full_path(char *path, char *name)
{
	char	*str;
	char	buf[PATH_MAX];

	if (!(str = getcwd(buf, PATH_MAX)))
		return (NULL);
	path = ft_strnew(PATH_MAX);
	ft_strclr(path);
	path = ft_strcpy(path, str);
	path = ft_strcat(path, "/");
	path = ft_strcat(path, name);
	return (path);
}