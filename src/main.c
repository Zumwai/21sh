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
/*
char	*exp_input(void)
{

	char *t;
	char	*head;
	char c;

	t = NULL;
	t = ft_strnew(200);
	head = t;
	ft_bzero(t, 200);
	while(1)
	{
		read(STDIN_FILENO, &c, sizeof(c));
		if (c == 127)
		{
			printf("\010 \010");
			*t = '\0';
			t--;
		}
		else if (c == '\n')
			return (head);
		else 
		{
			*t = c;
			t++;
		}
	}
	return (NULL);
}
*/

int		main(int ac, char **av, char **env)
{
	t_env	*ev;
	int		status;
	char	*line;

	if (ac < 1)
		return (-1);
	if (!av[0])
		return (-1);
	if (!(ev = create_env_list(env)))
		return (-1);
	init_tty();
	status = 1;
	line = NULL;
	while (status)
	{
		handle_all_signal(1);

		line = get_input();
		/*
		 For real this time
		 This is main branch
		 
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
	if (ev)
		delete_env_list(&ev);
	return (EXIT_SUCCESS);
}
