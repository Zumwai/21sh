#include "shell.h"

static void		init_tty(void)
{
	int		res;
	char	*tty_name;

	if (!(tty_name = getenv("TERM")))
		handle_exit_errors("terminal type is not defined\n");
	if (!isatty(STDIN_FILENO))
		handle_exit_errors("should be run in a terminal\n");
	res = tgetent(NULL, tty_name);
	if (res < 0)
		handle_exit_errors("could not access the termcap data base\n");
	if (!res)
		handle_exit_errors("specify a valid terminal name with setenv\n");
}

static t_yank	*init_buffer(void)
{
	t_yank *new;

	if(!(new = (t_yank *)malloc(sizeof(t_yank))))
		return (NULL);
	new->size = 0;
	new->yanked = 0;
	new->history = NULL;
	new->current = NULL;
	new->saved = NULL;
	new->hist_ptr = NULL;
	new->counter = 0;
	return (new);
}

t_env	*init_shell(int ac, char **av, char **env, t_yank **buffer)
{
	t_env	*ev;

	if (ac < 1)
		handle_exit_errors("Wrong number of arguments");
	if (!av[0])
		handle_exit_errors("Wrong number of arguments");
	init_tty();
	if (!(ev = create_env_list(env)))
		handle_exit_errors("Failed to crate ENV list");
	if (!(*buffer = init_buffer()))
		handle_exit_errors("Malloc returned NULL");
	return (ev);
}