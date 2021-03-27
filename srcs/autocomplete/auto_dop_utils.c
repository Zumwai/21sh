/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto_dop_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 11:53:27 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 12:23:46 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

extern char				*concat_pwd(char **path, char *orig, int len)
{
	char		*pwd;

	pwd = NULL;
	*path = ft_strnew(PATH_MAX);
	if (!(pwd = getcwd(pwd, PATH_MAX)))
		return (NULL);
	ft_strcat(*path, pwd);
	ft_strcat(*path, "/");
	ft_strncat(*path, orig, len - 1);
	ft_strcat(*path, "/");
	free(pwd);
	return (*path);
}

extern char				*split_path(char *orig, char **path)
{
	int		len;
	char	*sub;

	len = ft_strlen(orig) - 1;
	while (len >= 0)
	{
		if (orig[len] == '/')
			break ;
		len--;
	}
	len++;
	if (orig[0] == '/')
		*path = ft_strsub(orig, 0, len);
	else
		*path = concat_pwd(path, orig, len);
	sub = ft_strnew(257);
	if (len > 0)
		ft_strcpy(sub, &orig[len]);
	return (sub);
}

extern void				insert_var_dir(char *path, char *name, t_trie *head)
{
	struct stat		per;
	unsigned int	st;
	int				ret;
	char			tmp[PATH_MAX];

	ft_strclr(tmp);
	ft_memset(&per, 0, sizeof(struct stat));
	ft_strcpy(tmp, path);
	ft_strcat(tmp, "/");
	ft_strcat(tmp, name);
	ret = lstat(tmp, &per);
	if (ret != -1)
	{
		st = per.st_mode & S_IFMT;
		if (S_ISDIR(st))
			head = insert_word_trie(head, name);
	}
}

t_auto					*create_new_list(char *line)
{
	t_auto *new;

	if (!(new = (t_auto *)malloc(sizeof(t_auto))))
		handle_exit_errors("Malloc returned NULL");
	new->next = NULL;
	if (line)
	{
		new->name = ft_strdup(line);
		new->size = ft_strlen(line);
	}
	else
	{
		new->name = NULL;
		new->size = 0;
	}
	return (new);
}

extern t_trie			**init_array(void)
{
	t_trie	**node;
	int		i;

	i = 0;
	if (!(node = (t_trie **)malloc(sizeof(t_trie *) * 94)))
		handle_exit_errors("Malloc returned NULL");
	ft_bzero(node, sizeof(t_trie **) * 94);
	while (i < 94)
	{
		node[i] = NULL;
		i++;
	}
	return (node);
}
