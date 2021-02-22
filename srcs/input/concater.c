#include "sh.h"



char	*concat_lines(t_term *input)
{
	t_term	*curs;
	char	*line;
	char	*tmp;

	curs = input;
	line = NULL;
	while(curs)
	{
		tmp = line;
		if (curs->new) {
			
			//printf("%s\n", curs->new);
			if (!line && curs->new)
				line = ft_strdup(curs->new);
			else
				line = ft_strjoin(line, curs->new);
			free(tmp);
			//printf("%s\n", line);
		}
		curs = curs->next;
	}
	if (line && ft_strlen(line) > 4095)
	{
		handle_empty_error("usage: length", "input line too long, must be under 4096\n");
		set_free_null(&line);
		return (NULL);
	}
	return (line);
}
