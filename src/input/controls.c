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

static void go_next_word(char __attribute__((unused))*new, t_term *pos)
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