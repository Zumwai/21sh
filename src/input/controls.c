#include "shell.h"

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