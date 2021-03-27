#include "sh.h"

static void		init_tty(void)
{
	int		res;
	char	*tty_name;

	if (read(STDIN_FILENO, NULL, 0) == -1)
		handle_exit_errors("Shelp must have access to STDIN");
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

static void		init_tty_attr(t_yank *buf)
{
	struct termios	old_tty;
	struct termios	tty;

	tcgetattr(STDIN_FILENO, &old_tty);
	tcgetattr(STDIN_FILENO, &tty);
	//tty.c_lflag &= ~(ECHO | ICANON);
	tty.c_lflag &= ~(ECHO | ICANON | ISIG);
	//tty.c_oflag &= ~(TABDLY);
//	tty.c_lflag &= ~(ECHO | IEXTEN | ISIG);
	tty.c_cc[VMIN] = 0;
	tty.c_cc[VTIME] = 1;
	g_sig.old = old_tty;
	g_sig.work = tty;
	tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

static t_yank	*init_buffer(void)
{
	t_yank *new;

	if (!(new = (t_yank *)malloc(sizeof(t_yank))))
		return (NULL);
	ft_memset(new, 0, sizeof(t_yank));
	init_tty_attr(new);
	return (new);
}

t_env			*init_shell(int ac, char **av, char **env, t_yank **buffer)
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
