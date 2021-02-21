#include <stdio.h>
#include <string.h>
#include "./libft/libft.h"
# define	DEFAULT		0
# define	QUOTE		1<<0
# define	D_QUOTE		1<<1
# define	GLUE		1<<2
# define	REQ_HDOC	1<<3
# define	HEREDOC		1<<4
# define	ARG_HDOC	1<<5
# define	READ_HDOC	1<<6


typedef struct s_hdoc {
	int				cord;
	char			*eot;
	struct s_hdoc	*next;
}			t_hdoc;

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
		if (str[i] == '\\') {
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

int		verify_end_arg(char c, int n, int state)
{
	int		i = 0;
	if (c == '\\' && !n)
		return 0;
	if (c != '\\' && !n)
		return 1;
	if (c == '\'' || c == '\"') /* TMP solution for heredoc */
		return 1;
	if (verify_char_heredoc(c)) {
		if ((state & GLUE))
			return 0;
		return 1;
	}
	if (c == ' ')
		return 0; /* highly in doubts about whether 1 or 0 required */
	return 0;
}

t_hdoc	*create_new_hdoc(void)
{
	t_hdoc	*new;

	if (!(new = (t_hdoc *)malloc(sizeof(t_hdoc))))
		exit(1);
	new->cord = -1;
	new->eot = NULL;
	new->next = NULL;
}

void	save_coord_hdoc(t_hdoc **lst, int i)
{
	t_hdoc	*curs;

	if (*lst) {
		curs = *lst;
		while (curs->next)
			curs = curs->next;
		curs->next = create_new_hdoc();
		curs = curs->next;
	}
	else {
		curs = create_new_hdoc();
		*lst = curs;
	}
	curs->cord = i;
}

int		parse_incoming_subline(char *str, int prev, t_hdoc **del)
{
	int	i = 0;
	int	doc = 0;
	int	state = prev;
	char	c = 0;
	int		flag ;
	state &= ~(GLUE);
	if (!str[0]) { /* placeholer for empy line...for now */
		if ((state & READ_HDOC))
			state ^= READ_HDOC;
		return state;
	}
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
				state ^= READ_HDOC; /*return coordinate of the begining */
				if (!(state & HEREDOC))
					state ^= HEREDOC;
			}
		}
		if ((state & READ_HDOC))
		{
			/*if (verify_end_arg(str[i], str[i + 1], state)) || 
				state ^= READ_HDOC;
			if ((i == 0 && str[i] != '\\' && !str[i + 1]))
				state ^= READ_HDOC; */
			if ((verify_end_arg(str[i], str[i + 1], state) ||
				(i == 0 && str[i] != '\\' && !str[i + 1]))) {
					state ^= READ_HDOC;
					save_coord_hdoc(del, i);
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
				} else {
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
		} else if (!(state & REQ_HDOC) && !(state & ARG_HDOC) &&!(state & QUOTE) && !(state & D_QUOTE) && (state & HEREDOC) && str[i] == '<') {
			doc++;
			state |= REQ_HDOC;
		}
		else if (!state && doc == 2) {
			doc = 0;
			state ^= REQ_HDOC;
		} else if (!(state & REQ_HDOC) && !(state & ARG_HDOC) &&!(state & QUOTE) && !(state & D_QUOTE) && (state & HEREDOC) && doc == 2) {
			doc++;
			state |= REQ_HDOC;
		}
		i++;
	} 
	i--;
	if (!(state & QUOTE) && str[i] == '\\')
		state ^= (GLUE);
	return state;
}


char    *append_main_line(char *line, char *sub, int state)
{
    char    *new;
    int     size;
    int     sub_size;

    sub_size = ft_strlen(sub);
    size = sub_size + 1;
    if (line)
   		size += ft_strlen(line);
	if ((state & HEREDOC))
		size++;
    new = ft_strnew(size);
    if (line)
        ft_strcpy(new, line);
    ft_strcat(new, sub);
    if ((state & GLUE)) {
		new[size--] = 0;
		new[size--] = 0;
		new[size--] = 0;
		if ((state & HEREDOC)) {
			new[size] = 0;
			new[size] == '\n';
		}
	}
	else if ((state & HEREDOC))
		new[size - 2] = 10;
    return new;
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
	if (state & READ_HDOC)
		printf("currently reading heredoc delimeter!\n");
	printf("----------------------");
}

char	*grub_eot(char *line, int i)
{
	char	*eot= NULL;
	int		size = 0;

	while (i > 0)
	{
		if (line[i] == '<')
			break ;
		i--;
		size++;
	}

	eot = ft_strndup(&line[i + 1], size);
	return eot;
}

void	update_hdoc_list(t_hdoc **lst, char *line)
{
	t_hdoc	*curs;
	int		size;

	size = ft_strlen(line);
	if (!*lst)
		return ;
	curs = *lst;
	while (curs)
	{
		if (!curs->eot) {
			curs->cord += size;
			curs->eot = grub_eot(line, curs->cord);
		}
		curs = curs->next;
	}
}

int	main(void)
{
	int	state = 0;
	char		*line = NULL;
	char	ex[25] = "echo <<abc; echo <<124";
	char	ex2[10] = "123";
	char	ex3[25] = "456";
	char	ex4[11] = "abc";
	t_hdoc			*hdoc = NULL;
	char	*str;

	printf("~~~~~~~~~~~START~~~~~~~~~~~~~~\n");
	str = ft_strdup(ex);
	state = parse_incoming_subline(str, 0, &hdoc);
	line = append_main_line(line, str, state);
	update_hdoc_list(&hdoc, line);
	printer(state);
	printf("\n%s||FIRST!\n", line);

	char	*str2;
	str2 = ft_strdup(ex2);
	state = parse_incoming_subline(str2, state, &hdoc);
	line = append_main_line(line, str2, state);
	update_hdoc_list(&hdoc, line);
	printer(state);
	printf("\n%s||SECOND!\n", line);

	char	*str3;
	str3 = ft_strdup(ex3);
	state = parse_incoming_subline(str3, state, &hdoc);
	line = append_main_line(line, str3, state);
	update_hdoc_list(&hdoc, line);
	printer(state);
	printf("\n%s||THIRD!\n", line);
	char	*str4;
	str4 = ft_strdup(ex4);
	state = parse_incoming_subline(str4, state, &hdoc);
	line = append_main_line(line, str4, state);
	update_hdoc_list(&hdoc, line);
	printer(state);
	printf("\n%s||FORTH!\n", line);
	if (hdoc)
	{
		t_hdoc *curs = hdoc;
		while (curs)
		{
			printf("%d \n", curs->cord);
			printf("%s\n", curs->eot);
			curs = curs->next;
		}
	}
	return 0;
}

