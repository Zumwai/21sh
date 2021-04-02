#include "sh.h"

char 				*get_semantica_ret(char *line, int *i, char *res, int j)
{
	char			*ret;
	char			*tmp;

	ret = NULL;
	if (line[*i] == ';')
	{
		res[j + 1] = '\0';
		ret = ft_strtrim(res);
	}
	/*if (line[*i] == '\n')
	{
		res[j] = 'm';
		res[j + 1] = '\0';
		tmp = ft_strdup(res);
		ret = ft_strtrim(tmp);
		free(tmp);
		ft_putendl("sep");
		ft_putendl(ret);
		return (ret);
	}*/
	else if (line[*i] == '|')
	{
		ret = if_vertical_bar(line, i, res, j);
	}
	/*if (line[*i] == '&')
	{
		ret = if_ampersand(line, i, res, j);
		return (ret);
	}*/
	///if (line[*i] == '>' || line[*i] == '<')
	else if (line[*i] == '&')
	{
		ret = if_ampersand(line, i, res, j);
	}
	else if (line[*i] == '>' || line[*i] == '<')
	{
		ret = get_redirect(line, i, res, j);
	}
	return (ret);
}

int 				semantica(t_flag *flag, char *line, int *i, int *j)
{
	if (((flag->d_quot > 0) && (flag->d_quot % 2 != 0)) || ((flag->u_quot > 0)
															&& (flag->u_quot % 2 != 0)) ||
		(line[*i - 1] && line[*i - 1] == 92)) /// если открыты кавычки или есть бек-слэш
	{
		*i += 1;
		*j += 1;
		return (0);
	}
	if (*j != 0 || ((flag->d_quot % 2 != 0) || (flag->u_quot % 2 != 0) || /// если открыты кавычки или есть бек-слэш
					(line[*i - 1] && line[*i - 1] != 92)))
	{
	/*if (line[*i] == '&' && (line[*i + 1] == '>' || line[*i - 1] == '<' || line[*i - 1] == '>') || (line[*i] == '>' && (line[*i+ 1] == '&' || line[*i - 1] == '&')) ||
		(line[*i] == '<' && (line[*i + 1] == '&' || line[*i - 1] == '&')))
	{
		*i += 1;
		*j += 1;
		return (0);
	}
	else*/
		return (1);
	}
	if (*j == 0)
		return (2);
///	return (0);
///	return (-1); /* control path ZAGLUSHKO, fix pls ^^ */
///>>>>>>> master
}