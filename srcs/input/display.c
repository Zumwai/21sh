#include "sh.h"

static int calc_pos(t_term *pos, int cols)
{
	int diff_y;

	diff_y = 0;
	int diff_x = pos->delta_x;
	int tmp = ft_abs(pos->delta_x);

	if (tmp > pos->x)
	{
		while (tmp >= pos->x)
		{
			diff_y++;
			tmp -= cols;
			diff_x += cols;
		}
		pos->x += diff_x;
	}
	else
		pos->x += pos->delta_x;
	return diff_y;
}

static void calc_y_pos(t_term *pos, int diff)
{
	int printed = 0;
	if (diff && pos->y <= 0)
	{
		diff = pos->y + pos->delta_y - diff;
		if (diff <= 0)
		{
			int tmp = ft_abs(diff);
			printed = pos->y + pos->delta_y;
			int i = 0;
			if (pos->store->size)
			{
				while (i <= tmp)
				{
					tputs(tgoto(tgetstr("cm", NULL), 0, 0), 1, putchar_like);
					tputs(tgetstr("sr", NULL), 1, putchar_like);
					int pr = pos->store->size - i - 1 - (pos->y + pos->delta_y);
					if ((pr == 0 && !pos->prev) || pr < 0)
					{
						ft_putstr("shelp&>");
						ft_putstr(pos->store->arr[0]);
					}
					else
						ft_putstr(pos->store->arr[pr]);
					i++;
				}
			}
			tmp = ft_abs(diff);
			/* Change to zero for unduplicated lines */
			tmp += 1;
			t_term *curs;
			curs = pos;
			while (curs)
			{
				curs->y += tmp;
				curs = curs->prev;
			}
		}
	}
}

static void set_cursor(t_term *pos)
{
	struct winsize dimensions = {0};
	int diff_y;
	int counter = 0;
	diff_y = 0;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
	diff_y = calc_pos(pos, dimensions.ws_col);
	calc_y_pos(pos, diff_y);
	/*
	if (ft_abs(pos->y) - ft_abs(diff_y) + counter < 0)
	{
		tputs(tgetstr("sf", NULL), 1, putchar_like);
		counter++;
	}
	*/
	//tputs(tgetstr("cd", NULL), 1, putchar_like);
	tputs(tgoto(tgetstr("cm", NULL), pos->x, pos->y + pos->delta_y - diff_y - 1), 1, putchar_like);
}

static void correct_y(t_term *pos)
{
	t_term *curs = pos;

	while (curs)
	{
		curs->y--;
		curs = curs->prev;
	}
	curs = pos->next;
	while (curs)
	{
		curs->y--;
		curs = curs->next;
	}
}

static void handle_last_line(t_term *pos, int rows, int cols, int rem, int print)
{
	/* Scroll down and correct starting y in previous lines */
	if (pos->y + pos->delta_y >= rows)
	{
		if (pos->y > rows)
			pos->y = rows;
		correct_y(pos);
		tputs(tgetstr("sf", NULL), 1, putchar_like);
	}
	/* move cursor */
	if (pos->y + pos->delta_y > 0)
	{
		tputs(tgoto(tgetstr("cm", NULL), 0, pos->y + pos->delta_y), 1, putchar_like);
		pos->x = 0;
	}
	else
	{
		tputs(tgoto(tgetstr("cm", NULL), 0, 1), 1, putchar_like);
		pos->x = 0;
	}
	pos->delta_y++;
}

void append_arr(t_term *pos, char *line, int len)
{
	char **new;

	new = NULL;
	pos->store->size++;
	if (!(new = (char **)malloc(sizeof(char *) * (pos->store->size + 1))))
		handle_exit_errors("Malloc returned NULL");
	ft_memset(new, 0, sizeof(char *) * (pos->store->size + 1));
	int i = 0;
	if (pos->store->arr)
	{
		while (i < pos->store->size)
		{
			new[i] = pos->store->arr[i];
			i++;
		}
	}
	//pos->store->arr
	free(pos->store->arr);
	pos->store->arr = NULL;
	if (i > 0)
	{
		pos->store->size = i;
		i--;
	}
	new[i] = ft_strndup(line, len);
	pos->store->arr = new;
}

void ft_putstr_size(char *line, size_t size)
{
	write(1, line, size);
}

/*
Decides what is to be printed. If can't print, stores line in scrollback buffer.
Returns number of char left to print */

static int draw_line(t_term *pos, int remainder)
{
	int print = 0;
	int curr = 0;
	struct winsize dimensions;

	ioctl(STDIN_FILENO, TIOCGWINSZ, &dimensions);
	/* Calculate max possible character in this line */
	if (remainder == pos->index && !pos->prev)
		curr = 7;
	print = dimensions.ws_col - curr;
	/* Correct if line smaller than available space */
	if (print > remainder)
		print = remainder;
	/* Dont print anything beyond y < 0, just skip */

	if (pos->y + pos->delta_y > 0)
	{
		ft_putstr_size(&pos->new[pos->index - remainder], print);
		pos->x = print + curr;
	}
	//else
	//if ((print + curr) >= dimensions.ws_col)
	append_arr(pos, &pos->new[pos->index - remainder], print);
	/* Calculate diff and y for current printed part */
	if ((pos->next && remainder == print) || (print + curr == dimensions.ws_col))
		handle_last_line(pos, dimensions.ws_row, dimensions.ws_col, remainder, print + curr);
	if ((remainder - print) == 0)
		return -1;
	return (remainder - print);
}

static void set_empty_line(t_term *pos, int y, int prev)
{
	/*skip clearing if y < 0 */
	if (pos->y + pos->delta_y > 0)
	{
		tputs(tgetstr("cb", NULL), 1, putchar_like);
		tputs(tgetstr("cd", NULL), 1, putchar_like);
		tputs(tgoto(tgetstr("cm", NULL), 0, y + pos->delta_y - 1), 1, putchar_like);
		if (!prev)
			ft_putstr_fd("shelp$>", 1);
	}
	else
	{
		tputs(tgoto(tgetstr("cm", NULL), 0, 0), 1, putchar_like);
	}
}
/*
int display_input(t_term *curr, int delta)
{
	int remainder;
	//t_term	curr;

	if (!curr)
		return 0;
	//curr = *pos;
	remainder = curr->index;
	if (curr->prev)
		curr->y += delta;
	set_empty_line(curr, curr->y, !!curr->prev);
	while (remainder >= 0)
		remainder = draw_line(curr, remainder);
	if (!curr->next)
		tputs(tgetstr("cd", NULL), 1, putchar_like);
	set_cursor(curr);
	if (curr->next)
	{
		display_input(curr->next, curr->delta_y + delta);
	}
	int i = 0;
	while (curr->store->arr[i])
	{
		free(curr->store->arr[i]);
		curr->store->arr[i] = NULL;
		i++;
	}
	//if (pos->store->arr)
	//	ft_free_tab(pos->store->arr);
	if (curr->store->arr)
		free(curr->store->arr);
	curr->store->arr = NULL;
	curr->store->size = 0;
	curr->y -= delta;
	//if (curr.y != pos->y)
	//pos->y = curr.y;
	curr->delta_y = 0;
	//if (curr.y != pos->y)
	//*pos = curr;
	//	pos->y = curr.y - 1;
	return (0);
}
*/

int display_input(t_term *pos, int delta)
{
	int	remainder;
	t_term	curr;


	if (!pos)
		return 0;
	curr = *pos;
	remainder = curr.index;
	if (curr.prev)
		curr.delta_y += delta;
	set_empty_line(&curr, curr.y, !!curr.prev);
	while (remainder >= 0)
		remainder = draw_line(&curr, remainder);
	if (!curr.next)
		tputs(tgetstr("cd", NULL), 1, putchar_like);
	set_cursor(&curr);
	int tmp_y = 0;
	if (curr.next) {
		tmp_y = display_input(curr.next, curr.delta_y);
	}
	int	i = 0;
	while (curr.store->arr[i])
	{
		free(curr.store->arr[i]);
		curr.store->arr[i] = NULL;
		i++;
	}
	if (curr.store->arr)
		free(curr.store->arr);
	curr.store->arr = NULL;
	curr.store->size = 0;
	curr.delta_y = 0;
		if (tmp_y)
	curr.y = tmp_y;
	*pos = curr;
	//if (pos->next)
	if (tmp_y)
		pos->y = tmp_y;
	//if (curr.y != pos->y)
	//	pos->y = curr.y - 1;
	return (curr.y);
}

