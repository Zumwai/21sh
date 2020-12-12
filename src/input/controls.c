#include "shell.h"

static void key_exit(struct termios old_tty)
{
	tcsetattr(STDIN_FILENO, TCSADRAIN, &old_tty);
	exit(1);
}

static void move_left(t_term *pos)
{
	if ((ft_abs(pos->delta_x) - pos->index) != 0)
		pos->delta_x--;
	else
	{
		/* sound doesn't work */
		//ft_printf("%c", 7);
		//ft_printf("\a");
		tputs(tgetstr("bl", NULL), 1, putchar_like);
	}
}

static void move_right(t_term *pos)
{
	if	(pos->delta_x != 0)
			pos->delta_x++;
}

static void	backspace_char(char *new, t_term *pos)
{
	int abs = ft_abs(pos->delta_x);
	if (!pos->delta_x && pos->index)
	{
		ft_printf("\010 \010");
		new[--pos->index] = '\0';
		pos->x--;
	}
	else if (abs < pos->index)
	{
		ft_memmove(&new[pos->index - abs - 1], &new[pos->index - abs], abs);
		pos->index--;
		pos->x--;
	}
}

static void insert_char (char *new, t_term *pos, char c)
{
	int abs = ft_abs(pos->delta_x);

	ft_memmove(&new[pos->index - abs + 1], &new[pos->index - abs], abs);
	new[pos->index - abs] = c;
	pos->x++;
	pos->index++;
}

static void go_next_word(char *new, t_term *pos)
{
	int abs = ft_abs(pos->delta_x);

	int curs = pos->index - abs;
	while (new[curs] != '\0' && new[curs] == ' ')
		curs++;
	while (new[curs] && ft_ischar(new[curs]))
		curs++;
	curs = pos->index - curs;
	pos->delta_x = -curs;
	if (pos->delta_x > 0)
		pos->delta_x = 0;
}

static void go_prev_word(char *new, t_term *pos)
{
	int abs = ft_abs(pos->delta_x);

	int curs = pos->index - abs - 1;
	while (curs > 0 && new[curs] == ' ')
		curs--;
	while (curs > 0 && ft_ischar(new[curs]))
		curs--;
	if (curs > 0)
	{
		curs = pos->index - curs;
		pos->delta_x = -curs;
	}
	else
		pos->delta_x = -pos->index;
}

static void delete_char(char *new, t_term *pos)
{
	int abs = ft_abs(pos->delta_x);
	if (abs == pos->index && pos->index)
	{
		new = ft_memmove(new, &new[1], pos->index - 1);
		pos->index--;
		pos->delta_x++;
	}
	else if (abs && abs < pos->index)
	{
		ft_memmove(&new[pos->index - abs], &new[pos->index - abs + 1], abs);
		pos->index--;
		pos->delta_x++;
	}
}
static void change_line_down(t_term *pos)
{
	struct winsize dimensions;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);

	int tmp = pos->delta_x + dimensions.ws_col;

	if (tmp > 0)
		return ;
	else{
		pos->delta_x = tmp;
	//			pos->delta_y++;
	}
}

static void change_line_up(t_term *pos)
{
	struct winsize dimensions;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);

	int tmp = pos->delta_x - dimensions.ws_col;

	tmp = ft_abs(tmp);
	if (tmp > pos->index)
		return ;
	else
	{
		pos->delta_x = -tmp;
	//	pos->delta_y--;
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
			else if (key == L_WORD)
				go_prev_word(new, pos);
			else if (key == R_WORD)
				go_next_word(new, pos);
			else if (key == START)
				pos->delta_x = -pos->index;
			else if (key == END)
				pos->delta_x = 0;
			else if (key == L_UP )
				change_line_up(pos);
			else if (key == L_DOWN)
				change_line_down(pos);

			return (0);
}