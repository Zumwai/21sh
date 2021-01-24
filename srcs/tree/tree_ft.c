#include "sh.h"

static void 			*set_child(t_tree *child, t_tree *parent, int f)
{
	if (f == ext) /// правый
		parent->right = child;
	if (f == cmd) /// левый
		parent->left = child;
	child->parent = parent;
}

void 			*init_tree(void)
{
	t_tree 		*new;

	new = (t_tree *)malloc(sizeof(t_tree));
	new->token = NULL;
	new->left = NULL;
	new->right = NULL;
	new->parent = NULL;
	new->type = 0;
	return (new);
}