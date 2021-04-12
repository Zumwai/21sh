#include "sh.h"

int			sh_echo(t_cmd *com, t_env **env, int fd, int cmd)
{
	int			i;
	int			n;

	i = 1;
	n = 0;
	if (com->arr[i] && ft_strequ(com->arr[1], "-n"))
	{
		n = 1;
		i++;
	}
	while (com->arr[i])
	{
		ft_putstr_fd(com->arr[i], fd);
		if (com->arr[i + 1])
			ft_putchar_fd(' ', fd);
		i++;
	}
	if (!n)
		write(fd, "\n", 1);
	return 1;
}