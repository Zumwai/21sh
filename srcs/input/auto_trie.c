#include "sh.h"

static t_trie *create_trie_node(char c) {
    t_trie  *new;
    int     i;

    i = 0;
    if (!(new = (t_trie *)malloc(sizeof(t_trie))))
        handle_exit_errors("Malloc returned NULL");
    while (i < 94)
    {
        new->asc[i] = NULL;
        i++;
    }
    new->leaf = 0;
    new->data = c;
    return new;
}
 
static void free_trie_node(t_trie* node) {
    int     i;

    i = 0;
    while (i < 94)
    {
        if (node->asc[i] != NULL)
            free_trie_node(node->asc[i]);
        i++;
    }
    free(node);
}
static int  convert_asc_value(char c)
{
    if (c >= 48 && c <= 57)
        return c - 48;
    else if (c >= 65 && c <= 90)
        return c - 65 + 10;
    else if (c >= 97 && c <= 122)
        return c - 97 + 36;
    else
        return -1;    
}
/*void    insert_word_trie(t_trie *head, char *word)
{
    int     index;
    int     value;
    t_trie  *curs;

    curs = head;
    index = 0;
    value = 0;
    while (word[index])
    {
        value = convert_asc_value(word[index]);
        if (value < 0)
            handle_exit_errors("Trie value is negative\n");
        if (!curs->asc[value]) {
            curs->asc[value] = create_trie_node(word[index]);
        }
        curs = curs->asc[value];
     //   curs->leaf = 0;
    }
    curs->leaf = 1;
    return head;
}*/