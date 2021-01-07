#include "shell.h"

void	set_free_all(t_env *ev, t_yank *buffer)
{
	if (ev != NULL)
		delete_env_list(&ev);
	if (buffer->yanked != NULL)
		set_free_null(buffer->yanked);
	if (buffer->history)
		free_history(&(buffer->history));
	if (buffer != NULL)
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


void free_input_line(t_term *input)
{
	if (input->next)
		free_input_line(input->next);
	if (input->new)
		set_free_null(input->new);
	if (input->state == HEREDOC)
		set_free_null(input->substr);
	free(input);
	input = NULL;
}

void free_history(t_history **history)
{
	t_history *curs;
	t_history *tmp;

	curs = (*history);
	
	while (curs)
	{
		tmp = curs;
		if (curs->line)
			free_input_line(curs->line);
		curs = curs->next;
		free(tmp);
		tmp = NULL;
	}
}