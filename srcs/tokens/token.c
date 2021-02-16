#include "sh.h"

extern t_token 			*get_last_token(t_token *t)
{
	t_token 		*r;

	r = t;
	while (r->next != NULL)
		r = r->next;
	return (r);
}

static int 				right_row(t_token *t)
{
	t_token 		*first;

	first = t;
	if (first->type == 1)
	    return (0);
	while (first->next)
	{
		if (first->type == first->next->type)
			return (0);
		first = first->next;
	}
	return (1);
}

static int 				get_priority(char *s)
{
	//if (ft_strcmp(s, BK) == 0)
	//return ()
	if (ft_strcmp(s, OR) == 0)
		return (2);
	if (ft_strcmp(s, AND) == 0)
		return (2);
	if (ft_strcmp(s, PIPE) == 0)
		return (3);
	if (ft_strcmp(s, SC) == 0)
		return (1);
	/* Error */
	return 0;
}

static int					get_cmd_type(char *s)
{
	if (ft_strcmp(s, BK) == 0)
		return (bk);
	if (ft_strcmp(s, OR) == 0)
		return (ro);
	if (ft_strcmp(s, AND) == 0)
		return (and);
	if (ft_strcmp(s, PIPE) == 0)
		return (pi);
	if (ft_strcmp(s, SC) == 0)
		return (sc);
	if (ft_strcmp(s, GREAT) == 0)
		return (out_w);
	if (ft_strcmp(s, GGREAT) == 0)
		return (out_add);
	//if(ft_strcmp(s, LESS) == 0)
	//return ();
	//if(ft_strcmp(s, LLESS) == 0)
	//return ();

	/* Error */
	return 0;
}

extern int 				is_tokens_true(t_token *t)
{
	t_token 		*tmp;

	tmp = t;
	while (t)
	{
		if (ft_strcmp(t->data, BK) == 0 || ft_strcmp(t->data, SC) == 0 ||
				ft_strcmp(t->data, OR) == 0 ||
				ft_strcmp(t->data, AND) == 0 ||
				ft_strcmp(t->data, PIPE) == 0 ||
				ft_strcmp(t->data, GREAT) == 0 ||
				ft_strcmp(t->data, GGREAT) == 0 ||
				ft_strcmp(t->data, LESS) == 0 ||
				ft_strcmp(t->data, LLESS) == 0 )
		{
			t->type = 1;
			t->c_type = get_cmd_type(t->data);
			t->priority = get_priority(t->data);
		}
		else
			t->type = 0;
		t = t->next;
	}
	if (right_row(tmp))
		return (1);
	else
	    ft_putendl_fd("error of parsing", 2);
	return (0);
	/* error */
	return 0;
}

extern t_token 			*init_token(void)
{
	t_token		*new;

	if(!(new = (t_token *)malloc(sizeof(t_token))))
		return (NULL);
	new->data = NULL;
	new->priority = 0;
	new->next = NULL;
	new->prev = NULL;
	new->c_type = 0;
	new->type = 0;
	new->status = 0;
	return (new);
}