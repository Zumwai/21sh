/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 15:53:08 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 15:54:34 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sh.h"

static void		ft_clear(t_term *pos)
{
	ft_printf("%s", "\033[2J");
	tputs(tgoto(tgetstr("cm", NULL), 0, 1), 1, putchar_like);
	pos->y = 1;
}

static t_term	*get_curs(t_term *pos)
{
	t_term *curs;

	curs = NULL;
	curs = get_last_pos(pos);
	if (!curs->new)
		curs->new = get_buf_line(&curs->new, &curs->buf_size, 20);
	if (curs->index + 2 >= curs->buf_size)
		curs->new = get_buf_line(&curs->new, &curs->buf_size, 20);
	return (curs);
}

static void		norminate_cpypaste(t_term *curs, t_yank *buf, long long key)
{
	if (key == R_CUT)
		cut_after(curs, buf);
	else if (key == L_CUT)
		cut_before(curs, buf);
	else if (key == COPY_W)
		copy_word(curs, buf);
	else if (key == YANK)
		yank_buffer(curs, buf->yanked);
	else if (key == W_CUT)
		cut_word(curs, buf);
}

void			norminate_keyhandler(long long key, t_term *curs, t_yank *buf)
{
	if (key == BACKSPACE)
		backspace_char(curs);
	else if (key == DELETE)
		delete_char(curs);
	else if (key >= 32 && key <= 127)
		insert_char(curs, key);
	else if (key == RIGHT || key == LEFT || key == L_UP || key == L_DOWN)
		move_cursor(curs, key);
	else if (key == L_WORD)
		go_prev_word(curs);
	else if (key == R_WORD)
		go_next_word(curs);
	else if (key == START)
		curs->delta_x = -curs->index;
	else if (key == END)
		curs->delta_x = 0;
	else if (key == R_CUT || key == L_CUT
	|| key == COPY_W || key == YANK || key == W_CUT)
		norminate_cpypaste(curs, buf, key);
	else if (key == CLEAR)
		ft_clear(curs);
}

int 			read_key(long long key, t_term *pos, t_yank *buf, t_env **env)
{
	t_term	*curs;

	if (key == 27 || (key == 4 && (!pos->new || !pos->new[0])))
		return key_exit(pos, buf);
	if (key == 3)
		return (-6);
	curs = get_curs(pos);
	if (key == 4 && (!curs->new || !curs->new[0]))
		return (-5);
	if (key == ENTER)
		return (consult_state(curs));
	else if (key == HISTORY_UP)
		return (HIST_UP);
	else if (key == HISTORY_DOWN)
		return (HIST_D);
	else if (key == TAB)
		autocomplete(curs, env, buf);
	else
		norminate_keyhandler(key, curs, buf);
	return (1);
}