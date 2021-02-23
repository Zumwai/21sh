#include "sh.h"

/* UNUSED */
void		free_token_list(t_token **t)
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
			ft_free_tab(&curs->arr);
		if (curs->target)
			set_free_null(&curs->target);
		free(curs);
		curs = NULL;
		curs = tmp;
	}
}
/*
void			free_cmd_list(t_cmd **cmd)
{
	t_cmd		*curs;

	curs = *cmd;
	while (*cmd)
	{
		curs = *cmd;
		if ((*cmd)->next)
			*cmd = (*cmd)->next;
		if (curs->arr)
			ft_strsplit_free(&curs->arr);
		if (curs->target != NULL)
			free(curs->target);
		free(curs);
		curs = NULL;
	}
}
*/
/* Will be used later on temp env list for execve */
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

extern void	set_free_all(t_env *ev, t_yank *buffer)
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

void	ft_free_tab(char ***tab)
{
	int		i;
	char	**curs;

	curs = *tab;
	i = 0;
	if (!curs)
		return ;
	if (!curs[i])
		return ;
	while (curs[i])
	{
		if (curs[i])
			free(curs[i]);
		curs[i] = NULL;
		i++;
	}
	if (*tab)
		free(*tab);
	*tab = NULL;
}

void set_free_null(char **line)
{
	if (*line)
		free(*line);
	*line = NULL;
}
void		free_eot_list(t_hdoc **t)
{
	t_hdoc	*curs;
	t_hdoc	*tmp;

	curs = (*t);
	while (curs)
	{
		tmp = curs->next;
		if (curs->eot)
			set_free_null(&curs->eot);
		free(curs);
		curs = NULL;
		curs = tmp;
	}
	curs = NULL;
	*t = NULL;
}
void	free_main_line(t_actual **main)
{
	t_actual *curs;

	curs = *main;
	if(!(*main))
		return ;
	if ((*main)->hdoc)
		free_eot_list(&(*main)->hdoc);
	if ((*main)->line)
		set_free_null(&(*main)->line);
	(*main)->line = NULL;
	free(*main);
	*main = NULL;
	main = NULL;
}

void free_input_line(t_term **input)
{
	if (!(*input))
		return ;
	if (!(*input)->next) {
		free_main_line(&(*input)->main);
		(*input)->main = NULL;
	}
	if ((*input)->new)
		set_free_null(&(*input)->new);
	if ((*input)->store) {
		free((*input)->store);
		(*input)->store = NULL;
	}
	if ((*input)->next)
		free_input_line(&(*input)->next);
	free(*input);
	*input = NULL;
}

extern void free_history(t_history **history)
{
	t_history *curs;
	t_history *tmp;

	curs = (*history);
	
	while (curs)
	{
		tmp = curs;
		if (curs->line)
			free_input_line(&curs->line);
		curs = curs->next;
		free(tmp);
		tmp = NULL;
	}
}
