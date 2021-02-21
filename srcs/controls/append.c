#include "sh.h"

char    *append_main_line(char *line, char *sub, int state)
{
    char    *new;
    int     size;
    int     sub_size;

    sub_size = ft_strlen(sub);
    size = sub_size + 1;
    if (line)
   		size += ft_strlen(line);
    new = ft_strnew(size);

    if (line)
        ft_strcpy(new, line);
    ft_strcat(new, sub);
    if ((state & GLUE)) {
		int	i = 0;
		i = find_real_char(new, size);
		new[size] = 0;
		new[size - 1] = 0;
		new[size - 2] = 0;
	}
    return new;
}
