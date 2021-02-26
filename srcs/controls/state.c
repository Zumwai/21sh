#include "sh.h"


static char	find_next_char(char *str, int i)
{
	if (i < 0)
		return 0;
	while (str[i])
	{
		if (str[i] != ' ')
			return str[i];
		i++;
	}
	return 0;
}

static int	check_for_zero(char *str, int i)
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

static int		verify_end_arg(char c, int n, int state)
{
	int		i = 0;
	if (c == '\\' && !n)
		return 0;
	if (c != '\\' && !n)
		return 1;
	if (c == '\'' || c == '\"') /* TMP solution for heredoc, probably not enough */
		return 0;
	if (verify_char_heredoc(c)) {
		if ((state & GLUE))
			return 0;
		return 1;
	}
	if (c == ' ')
		return 1; /* highly in doubts about whether 1 or 0 required */
	return 0;
}

static int		parse_incoming_subline(char *str, int prev, t_hdoc **del, int size)
{
	int	i = 0;
	int	doc = 0;
	int	state = prev;
	char	c = 0;
	int		flag ;
	state &= ~(GLUE);
	if (!str[0]) {
		if ((state & READ_HDOC)) {
			state ^= READ_HDOC;
			save_coord_hdoc(del, i, size);
		}
		return state;
	}
	while (str[i]) {
		char b = str[i];
		if ((state & ARG_HDOC))
		{
			c = find_next_char(str, i);
			if (verify_char_heredoc(c)) { printf("failed verification 1%c\n", c);
				state |= FAILED;
			}
			else if (str[i] != ' ' && c != '\\' && !check_for_zero(str, i)) {
				state ^= ARG_HDOC;
				state ^= READ_HDOC;
				state |= HEREDOC;
			}
		}
		else if ((state & READ_HDOC))
		{
			if ((verify_end_arg(str[i], str[i + 1], state) ||
				(i == 0 && str[i] != '\\' && !str[i + 1]))) {
					state ^= READ_HDOC;
					save_coord_hdoc(del, i, size);
				}
		}
		else if ((state & REQ_HDOC)) {
			if (str[i] == '<') { /* dont need to check it here, useless and harmful */
				c = find_next_char(str, i + 1);
				if (verify_char_heredoc(c)) { printf("failed verification 2%c\n", c);
					state |= FAILED;
					break ;
					//return -1;
				}
				else if (c == '\\' && check_for_zero(str, i + 1)) {
					state ^= ARG_HDOC;
					state ^= REQ_HDOC;
				} else {/*useless ?*/
					state ^= ARG_HDOC; 
					state ^= REQ_HDOC;
				}
				if (str[1] == 0) {
					printf("failed sudden end of the line\n");
					state |= FAILED;
					break; /* useless? */
				}
			} else if (i > 0 && str[i] == '<' && str[i - 1] == '<' && !(state & QUOTE) && !(state & D_QUOTE)) {
				state ^= ARG_HDOC;
				state ^= REQ_HDOC;
			} else if (i == 0 && str[0] != '\\' && str[1]) {
				state |= FAILED;
				printf("failed glue 1\n");
				break ;
			}
			else if (str[i] != '<' && str[i + 1] != '\\' && str[i + 2] != 0) {
				printf("failed glue 2\n");
				state |= FAILED;
				break ; /*useles ?*/
			}
			else
				state ^= REQ_HDOC;
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
		} else if (!(state & REQ_HDOC) && !(state & ARG_HDOC) && !(state & QUOTE) && !(state & D_QUOTE) && (state & HEREDOC) && doc == 2) {
			doc = 0; /* probably an error? even if its going to happen, it will cause an error */
			//state |= REQ_HDOC;
		}
		i++;
	}
	i--;
	if (!(state & QUOTE) && str[i] == '\\')
		state |= (GLUE);
	if ((state & REQ_HDOC && !(state & GLUE))) {
		printf("failed glue 3\n");
		state |= FAILED;
	}
	return state;
}

/* calls to strlen too many times...*/
static void append_main_line(t_actual *main, char *sub, int state)
{
    char    *new = NULL;
	int		var = 0;
    int     size = 0;
    int     sub_size;

	size = 0;
	sub_size = 0;

	//vif ((state & HEREDOC))
	if (sub)
   		sub_size = ft_strlen(sub) + 1;
    size += sub_size;
	if (main->line)
		size += ft_strlen(main->line);
	if (size == 1)
		size++;
	if (!(state & GLUE) && ((state & HEREDOC) || (state & QUOTE) || (state & D_QUOTE)))
		size++;
    if (!(new = ft_strnew(size + 1)))
		handle_exit_errors("Malloc returned NULL");
    if (main->line)
        ft_strcat(new, main->line);
    ft_strcat(new, sub);
    if ((state & GLUE)) {
		new[size--] = 0;
		new[size--] = 0;
		new[size--] = 0;
	}
	else if ((state & HEREDOC) || (state & QUOTE) || (state & D_QUOTE)) {
		size--;
		new[size - 1] = '\n';
		new[size] = 0;
	}
	if (main->line) {
		free(main->line);
		main->line = NULL;
	}
	main->size = ft_strlen(new);
	main->line = new;
	main->line[size] = 0;
}

static t_term *create_next_io(t_actual **line, int y)
{
	t_term *input;

	input = create_new_io_struct(line);
	input->y = y;
	if (*line)
		input->main = *line;
	//input->new = get_buf_line(&input->new, &input->buf_size, 20);
	return input;
}

int		check_hdoc_eot(t_hdoc **head, char *sub)
{
	t_hdoc	*curs;

	curs = *head;
	if (!curs)
		return 1; /*probably will never happen*/
	while (curs)
	{
		if (curs->used == false)
		{
			if (ft_strequ(sub, curs->eot))
			{
				curs->used = true;
				if (!curs->next)
					return 0;
				return 1;
			}
			break ;
		}
		curs = curs->next;
	}
	return 1;
}

int		determine_next_io_step(t_term *curs, int ret)
{
	int		res = 1;
	if (curs->main->state & (FAILED))
		return -1; /* handle errors */
	if (!curs->main->state)
		return 0; /* check for ending */
	if ((curs->main->state & HEREDOC)) {
		res = update_hdoc_list(&curs->main->hdoc, curs->main->line);
		if (!res) {
			res = check_hdoc_eot(&curs->main->hdoc, curs->new);
		}
		if (res) {
			curs->next = create_next_io(&curs->main, curs->y);
			curs->next->prev = curs;
		}
	}
	else {
		curs->next = create_next_io(&curs->main, curs->y);
		curs->next->prev = curs;
	}
	return res;
}

int		consult_state(t_term *curs)
{
	int		ret = 0;
	int		small = 0;
	if (curs->prev) /* just to be safe */
		if (!curs->main)
			curs->main = create_main_line();
	ret = parse_incoming_subline(curs->new, curs->main->state, &curs->main->hdoc, ft_strlen(curs->main->line));
	curs->main->state_before = curs->main->state;
	curs->main->state = ret;
	append_main_line(curs->main, curs->new, curs->main->state);
	if (!(curs->main->state & FAILED))
		ret = determine_next_io_step(curs, ret);
	else {
		return (handle_return_error(-1, "syntax error near unexpected token `newline'\n"));
	}
	if (ret == 0)
		ft_putchar_fd('\n', 1);
	return ret;
}