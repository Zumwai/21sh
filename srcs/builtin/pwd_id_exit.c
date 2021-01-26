#include "sh.h"

int		sh_exit(void)
{
	return (0);
}

int	display_id_kid_parent(void)
{
	pid_t	kid;
	pid_t	par;

	ft_putstr("Current process id: ");
	kid = (getpid());
	ft_putnbr(kid);
	ft_putchar('\n');
	ft_putstr("Current process parent id: ");
	par = (getppid());
	ft_putnbr(par);
	ft_putchar('\n');
	return 1;
}

int		sh_pwd(void)
{
	char	*curr;

	curr = NULL;
	if (!(curr = getcwd(curr, PATH_MAX)))
		return (-1);
	ft_putstr(curr);
	ft_putchar('\n');
	free(curr);
	return (1);
}