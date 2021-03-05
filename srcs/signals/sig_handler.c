#include "sh.h"

static char *lookup_sig(int sig)
{
    char    *signame[] = {
        "SIGINT",
        "SIGQUIT",
        "SIGABRT",
        "SIGSTOP",
        "SIGCONT",
        "SIGCHILD"
    };
    if (sig == SIGINT)
        return (signame[0]);
    if (sig == SIGQUIT)
        return (signame[1]);
    if (sig == SIGABRT)
        return (signame[2]);
    if (sig == SIGSTOP)
        return (signame[3]);
    if (sig == SIGCONT)
        return (signame[4]);
    if (sig == SIGCHLD)
        return (signame[5]);
    return (NULL);
}

void	handle_child_signal(int sig)
{
    char    *ret;

    ret = NULL;
	if (sig)
	{
		ft_putstr("\n");
        ret = lookup_sig(sig);
        if (sig != SIGCHLD) {
                handle_empty_error("Signal recieved: ", ret);
            ft_putstr("\n");
        }
		signal(sig, handle_child_signal);
	}
}


int     check_sig(int sig)
{
    if (sig == SIGINT || sig == SIGABRT
     || sig == SIGSTOP || sig == SIGQUIT
      || sig == SIGCONT || sig == SIGSTOP
      || sig == SIGTSTP)
    return (1);
    return (0);
}

void	handle_main_signal(int sig)
{
    char    *ret;
    ret = NULL;
    if (sig == SIGWINCH)
    {
        if (g_sad->current)
            display_input(g_sad->current, 0);
    } else if (check_sig(sig)) {
        if (sig == SIGINT)
            ft_putstr("^C");
	//if (sig == SIGINT || sig == SIGABRT)
	//{
        //if (sig == SIGCHLD) {
            ret = lookup_sig(sig);
            if (ret == NULL)
                return ;
            handle_empty_error("Signal recieved: ", ret);
       // }
		ft_putstr("\n");
        if (sig != SIGCHLD)
		    ft_putstr("shelp$>");
		signal(sig, SIG_IGN);
	//}
    }
}

void	handle_all_signals(int pid)
{
	if (pid) {
		//signal(SIGINT, handle_main_signal);
    	signal(SIGABRT, handle_main_signal);
    	signal(SIGINT, handle_main_signal);
    	//signal(SIGSTOP, handle_main_signal);
    	signal(SIGCONT, handle_main_signal);
    	signal(SIGTSTP, handle_main_signal);
    	//signal(SIGKILL, handle_main_signal);
    	signal(SIGQUIT, handle_main_signal);
    	signal(SIGCHLD, handle_main_signal);
        signal(SIGWINCH, handle_main_signal);
        signal(SIGPIPE, handle_main_signal);
        signal(SIGTERM, handle_main_signal);
    }
	if (!pid) {
        //signal(SIGSTOP, handle_child_signal);
    	signal(SIGCONT, handle_child_signal);
		signal(SIGINT, handle_child_signal);
        signal(SIGABRT, handle_child_signal);
        signal(SIGQUIT, handle_child_signal);
        //signal(SIGKILL, handle_child_signal);
    }
}