#include <stdio.h>
#include <string.h>

# define	DEFAULT		0
# define	QUOTE		1<<0
# define	D_QUOTE		1<<1
# define	GLUE		1<<2
# define	REQ_HDOC	1<<3
# define	HEREDOC		1<<4
# define	ARG_HDOC	1<<5

int		parse_incoming_subline(char *str)
{
	int	i = 0;
	int	doc = 0;
	int	state = 0;

	while (str[i]) {
		if (str[i] == '\'')
			state ^= (QUOTE);
		if (str[i] == '\"')
			state ^= (D_QUOTE);
		if (!state && str[i] == '<')
			doc++;
		if (!state && doc == 2) {
			doc = 0;
			//state |= HEREDOC;
		}
		i++;
	}
	i--;
	if (str[i] == '\\')
		state |= (GLUE);
	if (!(state & QUOTE) && !(state & (D_QUOTE))) {
		if (state & GLUE)
			if (i > 0 && str[i - 1] == '<')
				state |= (REQ_HDOC);
	}
	return state;
}

int	main(void)
{
	int	i  = 0;
	int	doc = 0;
	int	state = 0;
	int	heredoc = 0;
	char	ex[] = "hello \" is this << a \' \'jojo reference? \\";
	char	*str;

	str = strdup((char *)ex);
	state = parse_incoming_subline(str);
	printf("%d - state\n", state);
	if (state & QUOTE)
		printf("quote!\n");
	if (state & D_QUOTE)
		printf("double quote!\n");
	if (state & GLUE)
		printf("GLUE!\n");
	if (state & REQ_HDOC)
		printf("seqond part for heredoc req!\n");
	if (state & HEREDOC)
		printf("heredoc found!\n");
	if (state & ARG_HDOC)
		printf("arg for heredoc required!\n");
	return 0;
}
