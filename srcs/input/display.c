#include "sh.h"

static int	calc_pos(t_term *pos, int cols)
{
	int	diff_y;

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
	} else 
		pos->x +=pos->delta_x;
	return diff_y;
}


static void calc_y_pos(t_term *pos, int diff)
{
	int		printed = 0;
	if (diff && pos->y <= 0)
	{
		diff = pos->y + pos->delta_y - diff;

		if (diff <= 0)
		{
			int tmp = diff;
			printed = pos->y + pos->delta_y;
			int i = 0;
			if (pos->store->size) {
			while (i <= tmp) {
				tputs (tgoto (tgetstr("cm", NULL), 0, 0), 1, putchar_like);
				tputs(tgetstr("sr", NULL), 1, putchar_like);
				if (pos->store->size - i - 1 == 0 && !pos->prev)
					ft_putstr("shelp&>");
				ft_putstr(pos->store->arr[pos->store->size - i - 1]);
				i++;
			}
			}
			tmp = diff;
			/* Change to zero for unduplicated lines */
			tmp += 1;
			t_term *curs;
			curs = pos;
			while (curs){
				curs->y += tmp;
				curs = curs->prev;
			}
		}
	}
}

static void  set_cursor(t_term *pos)
{
	struct winsize dimensions = {0};
	int		diff_y;
	int		counter = 0;
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
	tputs (tgoto (tgetstr("cm", NULL), pos->x, pos->y + pos->delta_y - diff_y - 1), 1, putchar_like);
}

static void	correct_y(t_term *pos)
{
	t_term *curs = pos;

	while (curs)
	{
		curs->y--;
		curs = curs->prev;
	}
}
static void	handle_last_line(t_term *pos, int rows, int cols, int rem, int print) 
{
	/* Scroll down and correct starting y in previous lines */
	if (pos->y + pos->delta_y >= rows) {
		if (pos->y > rows)
			pos->y = rows;
		correct_y(pos);
		tputs(tgetstr("sf", NULL), 1, putchar_like);
	}
	/* move cursor */
	if (pos->y + pos->delta_y > 0) {
		tputs(tgoto (tgetstr("cm", NULL), 0, pos->y + pos->delta_y), 1, putchar_like);
		pos->x = 0;
	}
	else {
		tputs(tgoto (tgetstr("cm", NULL), 0, 0), 1, putchar_like);
		pos->x = 0;
	}
	pos->delta_y++;
}

void	append_arr(t_term *pos, char *line, int len)
{
	char	**new;

	new = NULL;
	pos->store->size++;
	if (!(new = (char **)malloc(sizeof(char *) * pos->store->size + 1)))
		handle_exit_errors("Malloc returned NULL");
	ft_memset(new, 0, sizeof(char *) * pos->store->size + 1);
	int i = 0;
	if (pos->store->arr) {
		while (i < pos->store->size){
			new[i] = pos->store->arr[i];
			i++;
		}
	}
	if (pos->store->arr)
		free(pos->store->arr);
	pos->store->arr = NULL;
	pos->store->arr = new;
	pos->store->arr[pos->store->size - 1] = ft_strndup(line, len + 1);
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
	int	print = 0;
	int	curr = 0;
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

	if (pos->y + pos->delta_y > 0) {
		ft_putstr_size(&pos->new[pos->index - remainder], print);
		pos->x = print + curr;
	}
	else
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
	if (pos->y + pos->delta_y > 0) {
		tputs (tgoto (tgetstr("cm", NULL), 0, y - 1), 1, putchar_like);
		tputs(tgetstr("cb", NULL), 1, putchar_like);
		tputs(tgetstr("cd", NULL), 1, putchar_like);
		if (!prev)
			ft_putstr_fd("shelp$>", 1);
	} else {
		tputs (tgoto (tgetstr("cm", NULL), 0, 0), 1, putchar_like);
	}
}

/* delta currently is useless, always equals to zero */
int display_input(t_term *pos, int delta)
{
	t_term	*curs = pos;
	int	remainder;

	if (!pos)
		return 0;
	remainder = pos->index;
	if (pos->prev)
		pos->y += delta;
	set_empty_line(pos, pos->y, !!pos->prev);
	while (remainder >= 0)
		remainder = draw_line(pos, remainder);
	if (!pos->next)
		tputs(tgetstr("cd", NULL), 1, putchar_like);
	set_cursor(pos);
	if (curs->next) {
		display_input(curs->next, pos->delta_y + delta);
	}
	/* destruct scrollback buffer */
	if (pos->store->arr)
		ft_free_tab(&pos->store->arr);
	pos->store->arr = NULL;
	pos->store->size = 0;
	pos->y -= delta;
	pos->delta_y = 0;
	return (0);
}