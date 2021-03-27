#include "sh.h"
t_yank *g_sad;

int		main(int ac, char **av, char **env)
{
	t_token		*token;
	char		*line;
	t_cmd		*cmd;
	int			loop;
	int			fail;
	t_env	*ev;
	t_yank	*buffer;
	buffer = NULL;
	ev = init_shell(ac, av, env, &buffer);
	loop = 1;
	g_sad = 0;
	g_sad = buffer;
	while (loop > 0)
	{
		fail = 0;
		token = NULL;
		line = NULL;
		handle_all_signals(1);
		line = handle_input_stream(buffer, &ev, &fail);
		if (!line)
		{
			if (!(fail & (FAILED)))
				break ;
			else
				ft_putchar('\n');
		}
		else if (line[0] && !(fail & (FAILED)))
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
