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

static char	*get_buf_line(char **line, int *size)
{
	char	*new;

	struct winsize dimensions;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	new = ft_strnew(*size + dimensions.ws_col);
	*size += dimensions.ws_col;
	if (*line != NULL)
	{
		ft_strcpy(new, *line);
		free(*line);
		*line = NULL;
	}
	return (new);
}

/*
static int check_arrows(int key)
{
	if (key == 4479771)
	{
		tputs(tgetstr("#4", NULL), 1, putchar_like);
		return (1);
	}
	if (key == 4414235)
	{
		tputs(tgetstr("%i", NULL), 1, putchar_like);
		return (1);
	}
	return (0);
}
*/
static void key_exit(struct termios old_tty)
{
	tcsetattr(STDIN_FILENO, TCSADRAIN, &old_tty);
	exit(1);
}

static void draw_cursor_line(char *new, t_term *pos)
{
	int i = 0;
	int	tmp = pos->index + pos->prompt;
	struct winsize dimensions;
	//int tmp2 = tmp;
/*
	while (tmp2 > pos->x)
	{
		tputs(tgetstr("nd", NULL), 0, putchar_like);
		tmp2--;
	}
	*/
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	/*
	if (pos->x + 2 == dimensions.ws_col)
	{
		tputs(tgetstr("do", NULL), 1, putchar_like);
		pos->down = 1;
	}
	*/
	tputs(tgetstr("cb", NULL), 1, putchar_like);
	tputs(tgetstr("ce", NULL), 1, putchar_like);
	int left = pos->x;
	while (i++ < pos->prompt + pos->index){
		tputs(tgetstr("#4", NULL), 1, putchar_like);
		left--;
	}
	//while (left-- > 0)
	//	tputs(tgetstr("#4", NULL), 1, putchar_like);
	if (!pos->down)
		ft_putstr_fd("shelp$>", 1);
	ft_putstr_fd(new, 1);
	while (tmp-- > pos->x)
			tputs(tgetstr("le", NULL), 0, putchar_like);

//	pos->x = 7 + index;
//	pos->y = 0;
}

static void move_left(t_term *pos)
{
	if (pos->x > pos->prompt)
	{
		pos->x--;
	//	int i = pos->x;
	//	while (i++ < index + pos->prompt)
	//		tputs(tgetstr("le", NULL), 0, putchar_like);
	}
}

static void move_right(t_term *pos)
{
	if (pos->x < pos->index + pos->prompt)
	{
		pos->x++;
	//	int j = pos->x;
	//	while (j++ < index + pos->prompt)
	//		tputs(tgetstr("nd", NULL), 0, putchar_like);
	}
}

static void	backspace_char(char *new, t_term *pos)
{
	int		tmp = pos->index + pos->prompt;

	if (pos->x == tmp && pos->index > 0)
	{
		new[--pos->index] = '\0';
		pos->x--;
	}
	else if (pos->x > pos->prompt)
	{
		//new[pos->index] = '\0';
		//int curs = pos->index - 1 - tmp - pos->x;
		int curs = tmp - pos->x;
		curs = pos->index - curs - 1;
		while (new[curs] != '\0')
		{
				new[curs] = new[curs + 1];
				//new[curs] = '\0';
				curs++;
		}
		pos->index--;
		pos->x--;
		new[pos->index] = '\0';
	}
}

static void insert_char (char *new, t_term *pos, char c)
{
	int		tmp = pos->index + pos->prompt;
	if (pos->x == tmp && pos->index >= 0)
	{
		new[pos->index] = c;
		pos->x++;
		pos->index++;
	}
	else
	{
		/*
		int curs = tmp - pos->x;
		curs = pos->index - curs - 1;
		if (curs < 0)
			curs = 0;
		char	t1;
		int	t2;

		t1 = new[curs];
		pos->index++;
		t2 = pos->index;
		new[curs] = c;
		while (t2 >= 0 && curs <= t2 - 1)
		{
			new[t2] = new[t2 - 1];
			t2--;
		}
		if (t2 == -1)
			new[0] = 'Y';
		else if(t2 >= 0)
			new[t2] = t1;
		pos->x++;
		*/
		int curs = tmp - pos->x;
		curs = pos->index - curs;
		char	*sub;
		sub = ft_strsub(new, curs, tmp - pos->x + 1);
		new[curs] = c;
		new = ft_strcpy(&new[curs + 1], sub);
		pos->index++;
		pos->x++;
		free(sub);
		sub = NULL;
	}
}

static void go_next_word(char __attribute__((unused))*new, t_term __attribute__((unused))*pos)
{
	int		tmp = pos->index + pos->prompt;
	int curs = tmp - pos->x;
	curs = pos->index - curs;

//	printf("%d\n", curs);
	while (new[curs] != '\0' && new[curs] == ' ')
		curs++;
	while (new[curs] && ft_ischar(new[curs]))
		curs++;
//	curs--;
//	printf("%d\n", curs);
	pos->x = pos->prompt + curs;
}


static void go_prev_word(char *new, t_term *pos)
{

	//int		dif = 0;
	int		tmp = pos->index + pos->prompt;
	int curs = tmp - pos->x;
	curs = pos->index - curs - 1;
	/*
	if (curs <= 0)
	{
		pos->x = pos->prompt;
		return ;
	}
	//printf("%d\n", curs);

	int curs = pos->index + pos->prompt - pos->x;
	printf("%d\n", curs);
		*/
	while (curs > 0 && new[curs] == ' ')
		curs--;
	while (curs > 0 && ft_ischar(new[curs]))
		curs--;
	//curs++;
	//		printf("%d\n", curs);
	if (curs > 0)
		pos->x = curs + pos->prompt + 1;
	else
	{
		pos->x = pos->prompt;
	}
	
}


static void delete_char(char *new, t_term *pos)
{
	int		tmp = pos->index + pos->prompt;

	if (pos->x == pos->prompt && new[0])
	{
		char *sub;
		sub = ft_strdup(&new[1]);
	//	free(new);
		new = ft_strcpy(new, sub);
		new[pos->index] = '\0';
		free(sub);
		pos->index--;
	}
	else if (pos->x < tmp)
	{
		//new[pos->index] = '\0';
		//int curs = pos->index - 1 - tmp - pos->x;
		int curs = tmp - pos->x;
		curs = pos->index - curs;
		if (curs < 0)
			curs = 0;
		while (curs >= 0 && new[curs] != '\0')
		{
				new[curs] = new[curs + 1];
			//	new[curs] = '\0';
			//	printf("aa\n");
				curs++;
		}
		if (pos->index > 0)
			pos->index--;
	//	pos->x--;
		new[pos->index] = '\0';
	}
}

static char	*get_input(void)
{
	int		red;
	long long		key;
//	int		index;
	int		buf_size;
	char	*new;
	struct termios	old_tty;
	struct termios	tty;
	t_term	pos;

	tcgetattr(STDIN_FILENO, &old_tty);
	tcgetattr(STDIN_FILENO, &tty);
	
//	tty.c_cflag ~= |(ECHO);
	tty.c_lflag &= ~(ECHO | ICANON);
//	tty.c_lflag &= ~(ECHO | IEXTEN | ISIG);
	tty.c_cc[VMIN] = 1;
	tty.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSADRAIN, &tty);	
	ft_putstr_fd("shelp$>", 1);
	buf_size = 0;
	key = 0;
	new = NULL;
	pos.index = 0;
	pos.y = 0;
	pos.prompt = ft_strlen("shelp$>") + 1;
	pos.x = pos.prompt;
	pos.down = 0;
	new = NULL;
	while (1)
	{
			if (!new)
				new = get_buf_line(&new, &buf_size);
			//	new = ft_strnew(buf_size);
			if (pos.index + 2 >= buf_size)
			{
				new = get_buf_line(&new, &buf_size);
			//	buf_size+=20;
			}
			red = read(STDIN_FILENO, &key, sizeof(key));
		//	ft_putnbr(key);
		//	ft_putchar('\n');
		
		//	printf("%lld\n", key);
			if (key == 27)
				key_exit(old_tty);
			else if (key == BACKSPACE)
				backspace_char(new, &pos);
			else if (key == ENTER)
			{
				ft_putchar_fd('\n', 1);
				tcsetattr(STDIN_FILENO, TCSADRAIN, &old_tty);
				return (new);
			}
			else if (key == DELETE)
				delete_char(new, &pos);
			else if (key >= 32 && key <= 127)
				insert_char(new, &pos, key);
			else if (key == LEFT)
				move_left(&pos);
			else if (key == RIGHT)
				move_right(&pos);
			else if (key == START)
				pos.x = pos.prompt;
			else if (key == END)
				pos.x = pos.index + pos.prompt;
			else if (key == L_WORD)
				go_prev_word(new, &pos);
			else if (key == R_WORD)
				go_next_word(new, &pos);
			draw_cursor_line(new, &pos);
			red = 0;
			key = 0;
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &old_tty);
	return (new);
}
/*
void		interrogate_terminal(void)
{
	g_select.terminal.ptr = g_select.terminal.buf;
	g_select.terminal.start_cursor = tgetstr("ti", &g_select.terminal.ptr);
	g_select.terminal.stop_cursor = tgetstr("te", &g_select.terminal.ptr);
	g_select.terminal.show_cursor = tgetstr("ve", &g_select.terminal.ptr);
	g_select.terminal.hide_cursor = tgetstr("vi", &g_select.terminal.ptr);
	g_select.terminal.clear = tgetstr("cl", &g_select.terminal.ptr);
	g_select.terminal.underline = tgetstr("us", &g_select.terminal.ptr);
	g_select.terminal.end_underline = tgetstr("ue", &g_select.terminal.ptr);
	g_select.terminal.negative = tgetstr("mr", &g_select.terminal.ptr);
	g_select.terminal.normal = tgetstr("me", &g_select.terminal.ptr);
}
*/

void		init_tty(void)
{
	int		res;
	char	*tty_name;

//	g_select.terminal.fd = open(ttyname(STDIN_FILENO), O_RDWR | O_NOCTTY);
	if (!(tty_name = getenv("TERM")))
		handle_exit_errors("terminal type is not defined\n");
	if (!isatty(STDIN_FILENO))
		handle_exit_errors("should be run in a terminal\n");
	res = tgetent(NULL, tty_name);
	if (res < 0)
		handle_exit_errors("could not access the termcap data base\n");
	if (!res)
		handle_exit_errors("specify a valid terminal name with setenv\n");

	/* DONT NEED TO
	interrogate_terminal();
	tcgetattr(STDIN_FILENO, &g_select.terminal.saved_tattr);
	tcgetattr(g_select.terminal.fd, &g_select.terminal.tattr);

	g_select.terminal.tattr.c_lflag &= ~(ICANON | ECHO);
	g_select.terminal.tattr.c_cc[VMIN] = 1;

	tcsetattr(g_select.terminal.fd, TCSANOW, &g_select.terminal.tattr);
	tputs(g_select.terminal.start_cursor, 1, putchar_like);
	tputs(g_select.terminal.hide_cursor, 1, putchar_like);
	*/
}

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
