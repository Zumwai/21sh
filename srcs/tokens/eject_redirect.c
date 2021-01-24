#include "sh.h"

char			*get_redirect(char *line, int *i, char *res, int j)
{
	char		*ret;

	ret = NULL;
	if (line[*i] == line[*i + 1] || line[*i + 1] == '&')
		res[++j] = line[++*i];
	res[++j] = '\0';
	ret = ft_strtrim(ft_strdup(res));
	return (ret);
}