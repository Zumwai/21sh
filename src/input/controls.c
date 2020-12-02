#include "shell.h"

static void key_exit(struct termios old_tty)
{
	tcsetattr(STDIN_FILENO, TCSADRAIN, &old_tty);
	exit(1);
}

static void move_left(t_term *pos)
{
	if (pos->x > pos->prompt)
	{
		pos->x--;
	}
}

static void move_right(t_term *pos)
{
	if (pos->x < pos->index + pos->prompt)
	{
		pos->x++;
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
		int curs = tmp - pos->x;
		curs = pos->index - curs - 1;
		while (new[curs] != '\0')
		{
				new[curs] = new[curs + 1];
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
	if ((pos->x == tmp && pos->index >= 0) || pos->x == 0)
	{
		new[pos->index] = c;
		pos->x++;
		pos->index++;
	}
	else
	{
		struct winsize dimensions;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
		int curs = tmp - pos->x;
		curs = pos->index - curs;
		char	*sub;
	//	printf("%d\n%d\n", tmp, pos->x);
		sub = ft_strsub(new, curs, tmp - pos->x + 1);
		new[curs] = c;
		new = ft_strcpy(&new[curs + 1], sub);
		pos->index++;
		if (pos->index + 1 > dimensions.ws_col)
			pos->y++;
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
	
	while (new[curs] != '\0' && new[curs] == ' ')
		curs++;
	while (new[curs] && ft_ischar(new[curs]))
		curs++;
	pos->x = pos->prompt + curs;
}

static void go_prev_word(char *new, t_term *pos)
{
	int		tmp = pos->index + pos->prompt;
	int curs = tmp - pos->x;
	curs = pos->index - curs - 1;
	while (curs > 0 && new[curs] == ' ')
		curs--;
	while (curs > 0 && ft_ischar(new[curs]))
		curs--;
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
		new = ft_strcpy(new, sub);
		new[pos->index] = '\0';
		free(sub);
		pos->index--;
	}
	else if (pos->x < tmp)
	{
		int curs = tmp - pos->x;
		curs = pos->index - curs;
		if (curs < 0)
			curs = 0;
		while (curs >= 0 && new[curs] != '\0')
		{
				new[curs] = new[curs + 1];
				curs++;
		}
		if (pos->index > 0)
			pos->index--;
		new[pos->index] = '\0';
	}
}

int 	read_key(char *new, long long key, t_term *pos, struct termios old)
{
			if (key == 27)
				key_exit(old);
			else if (key == BACKSPACE)
				backspace_char(new, pos);
			else if (key == ENTER)
				return (-1);
			else if (key == DELETE)
				delete_char(new, pos);
			else if (key >= 32 && key <= 127)
				insert_char(new, pos, key);
			else if (key == LEFT)
				move_left(pos);
			else if (key == RIGHT)
				move_right(pos);
			else if (key == START)
				pos->x = pos->prompt;
			else if (key == END)
				pos->x = pos->index + pos->prompt;
			else if (key == L_WORD)
				go_prev_word(new, pos);
			else if (key == R_WORD)
				go_next_word(new, pos);
			return (0);
}