#include "sh.h"

t_cmd			*init_cmd(void)
{
	t_cmd		*new;

	if (!(new = (t_cmd *)malloc(sizeof(t_cmd))))
		handle_exit_errors("Malloc returned NULL");
	ft_memset(new, 0, sizeof(t_cmd));
	return (new);
}

extern t_cmd	*new_cmd(t_cmd *prev)
{
	t_cmd		*new;

	new = init_cmd();
	new->prev = prev;
	return (new);
}
