#include "sh.h"

extern char *builtin_list(int i)
{
    char    *arr[] = {
        "cd",
        "echo",
        "clear",
        "exit",
        "setenv",
        "unsetenv",
        "ppid",
        "pwd",
    };
    if (i > 6)
        return NULL;
    return arr[i];
}

extern void free_trie_node(t_trie* node) {
    int     i;

    i = 0;
    if (!node)
        return ;
    while (i < 95)
    {
        //if (node->asc && node->asc[i] != NULL){
        if (node->asc[i] != NULL){
            //if (node->counter == 1)
            free_trie_node(node->asc[i]);
          }
        i++;
    }
 //   if (node->asc)
   //     free(node->asc);
    if (node->sub)
        free(node->sub);
    if (node)
        free(node);
    node = NULL;
}

extern t_trie *create_trie_node(char c) {
    t_trie  *new;
    int     i;

    i = 0;
    if (!(new = (t_trie *)malloc(sizeof(t_trie))))
        handle_exit_errors("Malloc returned NULL");
    new->counter = 0;
    new->leaf = 0;
    new->data = c;
//    new->asc = NULL;
    while (i < 95)
    {
        new->asc[i]= NULL;
        i++;
    }
    new->sub = NULL;
    return new;
}
extern t_trie **init_array(void)
{
    int i = 0;
    t_trie  **node;
    if(!(node = (t_trie **)malloc(sizeof(t_trie *) * 94)))
        handle_exit_errors("Malloc returned NULL");
    bzero(node, sizeof(t_trie **) * 94);
    while (i < 94)
    {
        node[i] = NULL;
        i++;
    }
     return node;
}

extern int  convert_asc_value(char c)
{
    if (c >= 32 && c <= 127)
        return c - 32;
    else
        return -1;    
}

int is_relative_path(char *orig)
{
    int     i = 0;
    while (orig[i])
    {
        if (orig[i] == '/')
            return 1;
        i++;
    }
    return 0;
}

t_auto  *create_new_list(char *line)
{
    t_auto *new;

    if (!(new = (t_auto *)malloc(sizeof(t_auto))))
        handle_exit_errors("Malloc returned NULL");
    new->next = NULL;
    if (line) {
        new->name = ft_strdup(line);
        new->size = ft_strlen(line);
    }
    else
        new->name = NULL;
    return (new);
}