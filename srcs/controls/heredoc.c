#include "sh.h"

static t_hdoc	*create_new_hdoc(void)
{
	t_hdoc	*new;

	if (!(new = (t_hdoc *)malloc(sizeof(t_hdoc))))
		exit(1);
	new->cord = -1;
	new->eot = NULL;
    new->used = false;
	new->next = NULL;
}

extern void	save_coord_hdoc(t_hdoc **lst, int i, int size)
{
	t_hdoc	*curs;

	if (*lst) {
		curs = *lst;
		while (curs->next)
			curs = curs->next;
		curs->next = create_new_hdoc();
		curs = curs->next;
	}
	else {
		curs = create_new_hdoc();
		*lst = curs;
	}
	curs->cord = size + i;
}

static char	*grub_eot(char *line, int i)
{
	char	*eot= NULL;
	int		size = 0;

	while (i > 0)
	{
		if (line[i] == '<')
			break ;
		i--;
		size++;
	}
	eot = ft_strndup(&line[i + 1], size);
	if (verify_char_heredoc(eot[size - 1]))
		eot[size - 1] = 0;
	return eot;
}

extern void	update_hdoc_list(t_hdoc **lst, char *line)
{
	t_hdoc	*curs;
	int		size;

	size = ft_strlen(line);
	if (!*lst)
		return ;
	curs = *lst;
	while (curs)
	{
		if (!curs->eot) {
			curs->eot = grub_eot(line, curs->cord);
		}
		curs = curs->next;
	}
}