#include "sh.h"
t_control	g_sig;

int		main(int ac, char **av, char **env)
{
	t_token		*token;
	char		*line;
	t_cmd		*cmd;
	int			loop;
	t_env	*ev;
	t_yank	*buffer;

	buffer = NULL;
	ev = init_shell(ac, av, env, &buffer);
	loop = 1;
	while (loop >= 0)
	{
		token = NULL;
		line = NULL;
		handle_all_signals(1);
		line = handle_input_stream(buffer, &ev);
		if (!line)
		{
			if (!(buffer->state & (FAILED)))
				break ;
			else
				ft_putchar('\n');
		}
		else if (line[0] && !(buffer->state & (FAILED)))
		{
				token = parsing_t(line);
				if (token)
				{
					cmd = get_cmd(token, &ev);
					loop = execute(cmd, &ev, buffer);
					free_token_list(&token);
					free_cmd_list(&cmd);
				}
		}
		set_free_null(&line);
	}
	free_trie_node(buffer->trie);
	set_free_all(ev, buffer);
	return (0);
}
