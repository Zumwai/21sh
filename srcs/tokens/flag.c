#include "sh.h"

void 				update_flag(t_flag *flag, char s)
{
	if (s == '"')
		flag->d_quot += 1;
	if (s == '\'')
		flag->u_quot += 1;
}

t_flag				*reset_flag(t_flag *flag)
{
	flag->d_quot = 0;
	flag->u_quot = 0;
	return (flag);
}

t_flag				*init_flag(void)
{
	t_flag			*new;

	new = (t_flag *)malloc(sizeof(t_flag));
	new->d_quot = 0;
	new->u_quot = 0;
	return (new);
}