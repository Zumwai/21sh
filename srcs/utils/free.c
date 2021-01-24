#include "sh.h"

/* UNUSED */
void			free_token(t_token *t)
{
	t_token		*cur;

	cur = t;
	while (t)
	{
		cur = t;
		t = t->next;
		free(cur->data);
		free(cur);
	}
}

/* UNUSED */
void			free_cmd(t_cmd *cmd)
{
	t_cmd		*cur;

	while (cmd)
	{
		cur = cmd;
		cmd = cmd->next;
		if (cur->arr)
			ft_strsplit_free(&cur->arr);
		if (cur->target != NULL)
			free(cur->target);
		free(cur);
	}
}

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

void set_free_null(char **line)
{
	if (*line)
		free(*line);
	*line = NULL;
}


void free_input_line(t_term **input)
{
	if (!(*input))
		return ;
	if ((*input)->next)
		free_input_line(&(*input)->next);
	if ((*input)->new)
		set_free_null(&(*input)->new);
	if ((*input)->state == HEREDOC)
		set_free_null(&(*input)->substr);
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
