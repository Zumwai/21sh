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

static t_token 			*find_priority(t_token *t)
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

t_tree				*get_tree(t_token *t)
{
	t_tree 			*root;
	t_token 		*data;
	t_tree 			*cur;
	t_tree 			*cur_r;

	cur_r = NULL;
	data = get_last_token(t);
	root = init_tree();
	cur = root;
	while (data->priority != -1)
	{
		cur->token = find_priority(data);
		if (data->priority != -1)
		{
			cur->right = init_tree();
			cur->right->parent = cur;
			cur = cur->right;
		}
	}
	while (cur->parent != root)
	{
		if (cur->token->type == 0)
		{
			if (cur->right != NULL && cur->left != NULL && cur->parent)
			{
				cur = cur->parent;
				continue;
			}
			if (cur->right == NULL)
			{
				cur_r = init_tree();
				cur->right = cur_r;
				cur_r->parent = cur;
				cur_r->token = find_priority(data);
			}
			if (cur->right != NULL && cur->left == NULL)
			{
				cur_r = init_tree();
				cur->left = cur_r;
				cur_r->parent = cur;
				cur_r->token = find_priority(data);
			}
		}
		if (cur->parent && cur->token->type == 1)
		{
			if (cur->parent->right == NULL)
			{
				cur->parent->right = init_tree();
				cur_r = cur->parent->right;
			}
			if ((cur->parent->right != NULL) && cur->parent->left == NULL) {
				cur->parent->left = init_tree();
				cur_r = cur->parent->left;
			}
			cur_r->parent = cur->parent;
			cur_r->token = find_priority(data);
		}
		if (cur_r->token->type == 1)
			cur = cur->parent;
		if (cur_r->token->type == 0 && cur_r->right != NULL && cur_r->left != NULL)
			cur = cur_r->parent;
		if (cur_r->token->type == 0 && cur_r->left == NULL)
			cur = cur_r;
	}
	cur = root;
	if (root->token->prev)
	{
		cur->left = init_tree();
		cur_r = cur->left;
		cur_r->parent = cur;
		cur_r->token = find_priority(data);
		cur = cur_r;
	}
	while (root->token->prev->priority != -1)
	{
		cur->right = init_tree();
		cur->right->parent = cur;
		cur = cur->right;
		cur->token = find_priority(data);
	}
	while (cur_r->token != t)
	{
		if (cur->token->type == 0)
		{
			if (cur->right != NULL && cur->left != NULL && cur->parent)
			{
				cur = cur->parent;
				continue;
			}
			if (cur->right == NULL)
			{
				cur_r = init_tree();
				cur->right = cur_r;
				cur_r->parent = cur;
				cur_r->token = find_priority(data);
			}
			if (cur->right != NULL && cur->left == NULL)
			{
				cur_r = init_tree();
				cur->left = cur_r;
				cur_r->parent = cur;
				cur_r->token = find_priority(data);
			}
		}
		if (cur->parent && cur->token->type == 1)
		{
			if (cur->parent->right == NULL)
			{
				cur->parent->right = init_tree();
				cur_r = cur->parent->right;
			}
			if ((cur->parent->right != NULL) && cur->parent->left == NULL) {
				cur->parent->left = init_tree();
				cur_r = cur->parent->left;
			}
			cur_r->parent = cur->parent;
			cur_r->token = find_priority(data);
		}
		if (cur_r->token->type == 1)
			cur = cur->parent;
		if (cur_r->token->type == 0 && cur_r->right != NULL && cur_r->left != NULL)
			cur = cur_r->parent;
		if (cur_r->token->type == 0 && cur_r->left == NULL)
			cur = cur_r;
	}
	return (root);
}