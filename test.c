#include <stdio.h>
#include <string.h>

# define	DEFAULT		0
# define	QUOTE		1<<0
# define	D_QUOTE		1<<1
# define	GLUE		1<<2
# define	REQ_HDOC	1<<3
# define	HEREDOC		1<<4
# define	ARG_HDOC	1<<5

char	find_last_char(char *str, int i)
{
	if (i < 0)
		return 0;
	while (i > 0)
	{
		if (str[i] > 32 && str[i] < 127)
			return str[i];
		i--;
	}
}

char	find_next_char(char *str, int i)
{
	if (i < 0)
		return 0;
	while (str[i])
	{
		if (str[i] != ' ')
			return str[i];
		i++;
	}
}

int	check_for_zero(char *str, int i)
{
	if (i < 0)
		return 0;
	while (str[i])
	{
		if (str[i] = '\\') {
			if (str[ i + 1] == 0)
				return 1;
			else
				return 0;
		}
			break ;
		i++;
	}
	if (str[i] == 0)
		return 1;
	return 0;
}

int		verify_char_heredoc(char c)
{
	if (c == '|' || c == '&' || c == ';' || c == '\n' || c == 0 || c == '<' || c == '>')
		return 1;
	return 0;
}
int		parse_incoming_subline(char *str, int prev)
{
	int	i = 0;
	int	doc = 0;
	int	state = prev;
	char	c = 0;
	int		flag ;
	state &= ~(GLUE);
	//state &= ~HEREDOC;
	//if ((state & REQ_HDOC))
		//doc++;
	if (!str[0]) /* placeholer for empy line...for now */
		return -1;
	while (str[i]) {
		char b = str[i];
		if ((state & ARG_HDOC))
		{
			c = find_next_char(str, i);
			/* returns -1 if no args for heredoc present */
			if (verify_char_heredoc(c)) { printf("failed verification 1%c\n", c);
				return -1; }
			else if (c != '\\' && !check_for_zero(str, i)) {
				state ^= ARG_HDOC;
				//state ^= HEREDOC;
			}
		}
		else if ((state & REQ_HDOC)) {
			/*verifies that heredoc incoming or glue present*/
			if (str[i] == '<') {
				c = find_next_char(str, i + 1);
				if (verify_char_heredoc(c)) { printf("failed verification 2%c\n", c);
					return -1; }
				else if (c == '\\' && check_for_zero(str, i + 1)) {
					state ^= ARG_HDOC;
					state ^= REQ_HDOC;
				}else {
					state ^= ARG_HDOC;
					state ^= REQ_HDOC;
				}
				if (str[1] == 0)
					return -1;

			} else if (i > 0 && str[i] == '<' && str[i - 1] == '<' && !(state & QUOTE) && !(state & D_QUOTE)) {
				state ^= ARG_HDOC;
				state ^= REQ_HDOC;
			} else if (i == 0 && str[0] != '\\' && str[1]) {
				printf("failed glue 1\n");
				return -1;
			}
			else if (str[i] != '<' && str[i + 1] != '\\' && str[i + 2] != 0)
				return -1;
		}
		else if (str[i] == '\'' && !(state & D_QUOTE))
			state ^= (QUOTE);
		else if (str[i] == '\"'	&& !(state & QUOTE))
		state ^= (D_QUOTE);
		else if (!state && str[i] == '<') {
			doc++;
			state |= REQ_HDOC;
			/*
				c = find_next_char(str, i + 1);
				if (verify_char_heredoc(c)) { printf("failed verification %c\n", c);
					return -1; }
				else if (c != '\\' && !check_for_zero(str, i)) {
					state ^= ARG_HDOC;
				}
				if (str[1] == 0)
					return -1;
					*/
		}
		else if (!state && doc == 2) {
			doc = 0;
			state ^= REQ_HDOC;
			state |= ARG_HDOC;
			printf("Changed REQ TO ARG");
			/*shoud return to something and start from scratch */
		}
		i++;
	} 
	i--;
	/* commit to glue if not stated as single quote */
	if (!(state & QUOTE) && str[i] == '\\')
		state ^= (GLUE);
		/*
	 try to guess half cooked heredoc 
	if (!(state & QUOTE) && !(state & (D_QUOTE))) {
		c = find_last_char(str, i - 1);
	
		if (i > 0 && str[i] == '<') {
			if (state & GLUE) {
				if (!(state & HEREDOC))
				state |= (REQ_HDOC);
			} else
				return -1;
		}
	}
	*/
	return state;
}

void	printer(int state)
{
	printf("\n%d - state\n", state);
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
	printf("----------------------");
}

int	main(void)
{
	int	i  = 0;
	int	doc = 0;
	int	state = 0;
	int	heredoc = 0;

	//char	ex[] = "\"";
	//char	ex[] = "echo  < \\"; failure
	char	ex[] = "echo  <<abc ';' echo <<\\";
	char	*str;
	str = strdup((char *)ex);
	state = parse_incoming_subline(str, 0);
	printer(state);

	char	ex2[] = "\\";
	char	*str2;
	str2 = strdup((char *)ex2);
	state = parse_incoming_subline(str2, state);
	printer(state);


	char	ex3[] = "\\";
	char	*str3;
	str3 = strdup((char *)ex3);
	state = parse_incoming_subline(str3, state);
	printer(state);

	char	ex4[] = "\\";
	char	*str4;
	str4 = strdup((char *)ex4);
	state = parse_incoming_subline(str4, state);
	printer(state);
	return 0;
}
