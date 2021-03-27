#include "sh.h"

void			free_token_list(t_token **t)
{
	t_token	*curs;
	t_token	*tmp;

	curs = (*t);
	while (curs)
	{
		tmp = curs->next;
		if (curs->data)
			set_free_null(&curs->data);
		free(curs);
		curs = NULL;
		curs = tmp;
	}
}

void			free_cmd_list(t_cmd **cmd)
{
	t_cmd		*curs;
	t_cmd		*tmp;

	curs = (*cmd);
	while (curs)
	{
		tmp = curs->next;
		if (curs->arr)
			ft_free_tab(curs->arr);
		if (curs->target)
			set_free_null(&curs->target);
		curs->arr = NULL;
		free(curs);
		curs = NULL;
		curs = tmp;
	}
}

static void		delete_env_list(t_env **ev)
{
	t_env	*curs;
	t_env	*tmp;

	curs = (*ev);
	while (curs)
	{
		tmp = curs->next;
		if (curs->name)
			set_free_null(&curs->name);
		if (curs->value)
			set_free_null(&curs->value);
		free(curs);
		curs = NULL;
		curs = tmp;
	}
}

extern void		set_free_all(t_env *ev, t_yank *buffer)
{
	if (ev != NULL)
		delete_env_list(&ev);
	if (buffer->yanked != NULL)
		set_free_null(&buffer->yanked);
	if (buffer->history)
		free_history(&(buffer->history));
	if (buffer != NULL)
	{
		free(buffer);
		buffer = NULL;
	}
}

void			ft_free_tab(char **tab)
{
	int		i;
	int		j;
	char	**curs;

	curs = tab;
	i = 0;
	j = 0;
	if (!curs)
		return ;
	while (curs[i])
	{
		free(curs[i]);
		i++;
	}
	if (tab)
		free(tab);
	tab = NULL;
}
