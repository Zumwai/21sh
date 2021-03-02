#include "sh.h"


static t_auto  *find_last(t_auto *head)
{
    t_auto *curs;

    curs = head;
    if (!curs)
        return NULL;
    while (curs->next)
            curs = curs->next;
    return curs;
}

void    print_words(t_trie *node, char **line, int index, t_auto *list)
{
    if (!node)
        return ;
    if (line[0][index])
        line[0][index] = 0;
    if (node->data != -1)
    {
        if (node->sub) {
            ft_strcat(*line, node->sub);
            index += ft_strlen(node->sub);
        } else {
            line[0][index] = node->data;
            line[0][index + 1] = '\0';
            index++;
        }
    }
    if (node->leaf == true) {
        t_auto *curs;
        curs = find_last(list);
        curs->next = create_new_list(*line);
    }
    int i = 0;
//    if (node->asc) {
    while (i < 94)
    {
        if (node->asc[i]) {
            print_words(node->asc[i], line, index, list);
        }
        i++;
    }
 //   }
}

void    print_varians(t_term *pos, t_auto *list)
{
    t_auto *curs = list;
    int     size = 0;
    int     max = 0;
	struct winsize dimensions;
    int     cols = 0;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &dimensions);
    while (curs)
    {
        size++;
        if (curs->size > max)
            max = curs->size;
        curs = curs->next;
    }
    max += 1;
    curs = list;
    if (max)
        cols = dimensions.ws_col / max;
    int     i = 0;
    t_auto *tmp;
    ft_putchar('\n');
    while (curs)
    {
        tmp = curs;
        curs = curs->next;
        ft_putstr(tmp->name);
        while (tmp->size++ < max)
            ft_putchar(' ');
        i++;
        if (i == cols) {
            ft_putchar('\n'); i = 0;
        }
        free(tmp->name);
        free(tmp);
    }
    ft_putchar('\n');
    update_coord(pos);
}