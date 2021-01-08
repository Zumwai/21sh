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

int		main(int ac, char **av, char **env)
{
	t_env	*ev;
	int		status;
	char	*line;
	t_yank	*buffer;

	buffer = NULL;
	ev = init_shell(ac, av, env, &buffer);
	status = 1;
	while (status)
	{
		handle_all_signal(1);
		line = handle_input_stream(buffer);
		status = register_input(&ev, line);
		set_free_null(&line);
		if (!status || status == FIN)
			break ;
	}
	set_free_all(ev, buffer);
	return (EXIT_SUCCESS);
}
