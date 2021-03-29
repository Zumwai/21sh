#include "sh.h"

int			sh_echo(char **com, t_env **env, int fd, int cmd)
{
	int			i;
	int			n;

	i = 1;
	n = 0;
	if (com[i] && ft_strequ(com[1], "-n"))
	{
		n = 1;
		i++;
	}
	while (com[i])
	{
		ft_putstr_fd(com[i], fd);
		if (com[i + 1])
			ft_putchar_fd(' ', fd);
		i++;
	}
	if (!n)
		write(fd, "\n", 1);
	return 1;
}