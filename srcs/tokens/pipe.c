#include "sh.h"

char 			*if_vertical_bar(char *line, int *i, char *res, int j)
{
	char 		*ret;
	char		*buf;

	ret = NULL;
	if (line[*i + 1] == '|')
	{
		res[++j] = line[++*i];
		res[++j] = '\0';
	}
	else
		res[++j] = '\0';
	buf = ft_strdup(res);
	ret = ft_strtrim(buf);
	free(buf);
	return (ret);
}
