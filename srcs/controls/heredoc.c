#include "sh.h"

static t_hdoc	*create_new_hdoc(void)
{
	t_hdoc	*new;

	if (!(new = (t_hdoc *)malloc(sizeof(t_hdoc))))
		handle_exit_errors("Malloc returned NULL");
	new->cord = -1;
	new->eot = NULL;
    new->used = false;
	new->next = NULL;
    return new;
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

	while(line[i] == ' ') {
		i--;
	}
	while (i > 0)
	{
		if (line[i] == '<' || line[i] == ' ')
			break ;
		i--;
		size++;
	}
	eot = ft_strndup(&line[i + 1], size);
	if (verify_char_heredoc(eot[size - 1]))
		eot[size - 1] = 0;
	return eot;
}

extern int	update_hdoc_list(t_hdoc **lst, char *line)
{
	t_hdoc	*curs;
	int		size;
	int		ret = 0;

	size = ft_strlen(line);
	if (!*lst)
		return 0; /*not sure*/
	curs = *lst;
	while (curs)
	{
		if (!curs->eot) {
			curs->eot = grub_eot(line, curs->cord);
			ret = 1;
		}
		curs = curs->next;
	}
	return ret;
}

extern t_hdoc *clone_hdoc(t_hdoc *old)
{
    t_hdoc *new;
    t_hdoc *head;

    new = create_new_hdoc();
    head = new;
    while (old)
    {
        new->cord = old->cord;
        new->used = old->used;
        new->eot = ft_strdup(old->eot);
        old = old->next;
        if (old) {
            new->next = create_new_hdoc();
            new = new->next;
        }
    }
    return head;
}