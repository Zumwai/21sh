/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 20:10:47 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 20:11:54 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static char		*lookup_sig(int sig)
{
	if (sig == SIGINT)
		return ("SIGINT");
	if (sig == SIGQUIT)
		return ("SIGQUIT");
	if (sig == SIGABRT)
		return ("SIGABRT");
	if (sig == SIGSTOP)
		return ("SIGSTOP");
	if (sig == SIGCONT)
		return ("SIGCONT");
	if (sig == SIGCHLD)
		return ("SIGCHILD");
	return (NULL);
}

void			handle_child_signal(int sig)
{
	char	*ret;

	ret = NULL;
	if (sig)
	{
		ft_putstr("\n");
		ret = lookup_sig(sig);
		if (sig != SIGCHLD)
		{
			handle_empty_error("Signal recieved: ", ret);
			ft_putstr("\n");
		}
		signal(sig, handle_child_signal);
	}
}

int				check_sig(int sig)
{
	if (sig == SIGINT || sig == SIGABRT
	|| sig == SIGSTOP || sig == SIGQUIT
	|| sig == SIGCONT || sig == SIGSTOP
	|| sig == SIGTSTP)
		return (1);
	return (0);
}

void			handle_main_signal(int sig)
{
	struct winsize	dimensions;
	char			*ret;

	ret = NULL;
	if (sig == SIGWINCH)
	{
		g_sig.winch = 1;
	}
	else if (check_sig(sig))
	{
		if (sig == SIGINT)
			ft_putstr("^C");
			ret = lookup_sig(sig);
			if (ret == NULL)
				return ;
			handle_empty_error("Signal recieved: ", ret);
		ft_putstr("\n");
		if (sig != SIGCHLD)
			ft_putstr("shelp$>");
		signal(sig, SIG_IGN);
	}
}

void			handle_all_signals(int pid)
{
	if (pid)
	{
		//signal(SIGINT, handle_main_signal);
		signal(SIGABRT, handle_main_signal);
		//signal(SIGINT, handle_main_signal);
		//signal(SIGSTOP, handle_main_signal);
		signal(SIGCONT, handle_main_signal);
		//signal(SIGTSTP, handle_main_signal);
		//signal(SIGKILL, handle_main_signal);
		signal(SIGQUIT, handle_main_signal);
		signal(SIGCHLD, handle_main_signal);
		signal(SIGWINCH, handle_main_signal);
		signal(SIGPIPE, handle_main_signal);
		//signal(SIGTERM, handle_main_signal);
	}
	if (!pid)
	{
		//signal(SIGSTOP, handle_child_signal);
		signal(SIGCONT, handle_child_signal);
		signal(SIGINT, handle_child_signal);
		signal(SIGABRT, handle_child_signal);
		signal(SIGQUIT, handle_child_signal);
		//signal(SIGKILL, handle_child_signal);
	}
}
