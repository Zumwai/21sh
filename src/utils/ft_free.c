#include "shell.h"

void	set_free_all(t_env *ev, t_yank *buffer)
{
	if (ev)
		delete_env_list(&ev);
	if (buffer->yanked)
		set_free_null(buffer->yanked);
	if (buffer)
	{
		free(buffer);
		buffer = NULL;
	}
	
}

void	ft_free_tab(char **tab)
{
	int		i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		tab[i] = NULL;
		i++;
	}
	free(tab);
	tab = NULL;
}

void set_free_null(char *line)
{
	if (line)
		free(line);
	line = NULL;
}