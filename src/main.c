/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/29 14:42:08 by student           #+#    #+#             */
/*   Updated: 2020/07/29 15:32:54 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

t_yank	*init_buffer(void)
{
	t_yank *new;

	if(!(new = (t_yank *)malloc(sizeof(t_yank))))
		return (NULL);
	new->size = 0;
	new->yanked = 0;
	return (new);
}

int		main(int ac, char **av, char **env)
{
	t_env	*ev;
	int		status;
	char	*line;
	t_yank	*buffer;

	if (ac < 1)
		return (-1);
	if (!av[0])
		return (-1);
	if (!(ev = create_env_list(env)))
		return (-1);
	init_tty();
	buffer = init_buffer();
	if (!buffer)
		handle_exit_errors("Malloc returned NULL");
	status = 1;
	line = NULL;
	while (status)
	{
		handle_all_signal(1);
		line = get_input(buffer);
		if (!line)
			break ;
		/*
		 For real this time
		 This is INPUT branch
		 
		line = exp_input();
		*/
		status = register_input(&ev, line);
		if (line)
		{
			free(line);
			line = NULL;
		}
		if (!status || status == FIN)
			break ;
	}
	set_free_all(ev, buffer);

	return (EXIT_SUCCESS);
}
