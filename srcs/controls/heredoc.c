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

static	char *trim_inword(char *line, int size, int count)
{
	char	*ret = NULL;
	int		i = 0;
	int		j = 0;
	if (count)
	{
		ret = ft_strnew(size - count);
		while (line[i])
		{
			if (line[i] != '\'' || line[i] != '\"') {
				ret[j] = line[i];
				j++;
			}
			i++;
		}
		free(line);
		return ret;
	}
	return line;
}
static char	*grub_eot(char *line, int i)
{
	char	*eot= NULL;
	int		count = 0;
	int		size = 0;
	char	*eot2 = NULL;
	while(line[i] == ' ') {
		i--;
	}
	while (i > 0)
	{
		if (line[i] == '<' || line[i] == ' ')
			break ;
		if (line[i] == '\'' || line[i] == '\"')
			count++;
		i--;
		size++;
	}
	eot = ft_strndup(&line[i + 1], size);
	if (verify_char_heredoc(eot[size - 1]))
		eot[size - 1] = 0;
	if (count)
		eot = trim_inword(eot, size, count);
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
	new->next = NULL;
    while (old)
    {
        new->cord = old->cord;
        new->used = old->used;
        new->eot = ft_strdup(old->eot);
		new->next = NULL;
        old = old->next;
		if (old) {
			new->next = create_new_hdoc();
			new = new->next;
		}
		/*
        if (old) {
			if (old->next) {
            	new->next = create_new_hdoc();
            	new = new->next;
			}
        }
		*/
    }
	new->used = false;
    return head;
}

/*
extern t_hdoc *clone_hdoc(t_hdoc *old)
{
	t_hdoc *new = NULL;
	t_hdoc *head = NULL;

	while (old)
	{
		if (old->next){
			if (!new)
			{
				new = create_new_hdoc();
				head = new;
			} else {
				new->next = create_new_hdoc();
				new = new->next;
			}
			new->cord = old->cord;
			new->used = old->used;
			new->eot = ft_strdup(old->eot);
		}
		old = old->next;
	}
	return head;
}
*/