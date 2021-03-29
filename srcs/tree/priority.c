#include "sh.h"

static t_token 			*third_priority(t_token *t)
{
	while (t && t->priority != -1)
	{
		if (ft_strcmp(t->data, PIPE) == 0 && t->priority != -1)
		{
			t->priority = -1;
			return (t);
		}
		t = t->prev;
	}
	return (NULL);
}

static t_token 			*second_priority(t_token *t)
{
	while (t && t->priority != -1)
	{
		if ((ft_strcmp(t->data, OR) == 0 || ft_strcmp(t->data, AND) == 0) && t->priority != -1)
		{
			t->priority = -1;
			return (t);
		}
		t = t->prev;
	}
	return (NULL);
}

static t_token 			*first_priority(t_token *t)
{
	while (t && t->priority != -1)
	{
		if (ft_strcmp(t->data, SC) == 0 && t->priority != -1)
		{
			t->priority = -1;
			return (t);
		}
		t = t->prev;
	}
	return (NULL);
}

t_token 			*find_priority(t_token *t)
{
	t_token			*cur;
	t_token 		*res;

	if (t == NULL)
		return (NULL);
	while (t && t->priority == -1)
		t = t->prev;
	cur = t;

		res = first_priority(cur);
		if (res != NULL)
		{
			res->priority = -1;
			return (res);
		}
		res = second_priority(cur);
		if (res != NULL) {
			res->priority = -1;
			return (res);
		}
		res = third_priority(cur);
		if (res != NULL) {
			res->priority = -1;
			return (res);
		}
		if (res == NULL)
		{
			if (cur)
				cur->priority = -1;
			return (cur);
		}
	/* error */
	return NULL;
}