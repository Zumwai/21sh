#include "sh.h"

char 			*if_ampersand(char *line, int *i, char *res, int j)
{
	char		*ret;

	ret = NULL;
	if (line[*i + 1] == '&')
		res[++j] = line[++*i];
	res[++j] = '\0';
	ret = ft_strtrim(ft_strdup(res));
	return (ret);
}