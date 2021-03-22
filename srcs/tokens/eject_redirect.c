#include "sh.h"

char			*get_redirect(char *line, int *i, char *res, int j)
{
	char		*ret;
	char		*tmp;

	ret = NULL;
	if (line[*i] == line[*i + 1])
		res[++j] = line[++*i];
	res[++j] = '\0';
	tmp = ft_strdup(res);
	ret = ft_strtrim(tmp);
	free(tmp);
	return (ret);
}