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