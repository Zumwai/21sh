#include "sh.h"

t_tree		*get_to_priority(t_token *data, t_tree *cur)
{
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
	return (cur);
}

t_tree				*handle_zero_type(t_tree * cur, t_token *data)
{
	t_tree	*cur_r;

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
	return (cur);
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
	cur = get_to_priority(data, cur);
	while (cur->parent != root)
	{
		if (cur->token->type == 0)
		{
			if (cur->right != NULL && cur->left != NULL && cur->parent)
			{
				cur = cur->parent;
				continue;
			}
			cur = handle_zero_type(cur, data);
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