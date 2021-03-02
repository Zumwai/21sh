#include "sh.h"

static t_trie    *insert_word_trie(t_trie *head, char *word)
{
    int     index;
    int     value;
    t_trie  *curs;

    if (!head)
        head = create_trie_node(0);
    curs = head;
    index = 0;
    value = 0;
    while (word[index])
    {
        value = convert_asc_value(word[index]);
        if (value < 0)
            handle_exit_errors("Trie value is negative!\n");
//        if (!curs->asc) {
//            curs->asc = init_array();
//        }
        if (!curs->asc[value]) {
            curs->asc[value] = create_trie_node(word[index]);
            curs->counter++;
            curs = curs->asc[value];
            if (word[index + 1]) {
                curs->sub = ft_strdup(&word[index]);
//                curs->asc = NULL;
            }
            else {
                curs->data = word[index];
            }
            curs->leaf = 1;
            return head;
        } else {
            curs = curs->asc[value];
            if (curs->sub)
            {
                if (curs->sub[1])
                    insert_word_trie(curs, &curs->sub[1]);
                  set_free_null(&curs->sub);
                  //curs->counter--;
                  curs->leaf = 0;
            }
        }
		curs->data = word[index];
        //curs->counter++;
		index++;
    }
    curs->leaf = 1;
    return head;
}

static t_trie *fill_variant_list(char *orig, char *path, t_trie *head)
{
    DIR     *dir;
    struct dirent *container;
    int     len;

    len = 0;
    dir = opendir(path);
    if (!dir)
        return head;
    if (!orig)
        return NULL;
    len = ft_strlen(orig);
    while ((container = readdir(dir)))
    {
        if(container->d_reclen == 0)
            break ; //test
        if ((orig[0] == 0 || ft_strnequ(container->d_name, orig, len)) && ft_strcmp(container->d_name, ".") && ft_strcmp(container->d_name, "..")) {
            head = insert_word_trie(head, container->d_name);
        }
    }
    free(dir);
    return head;
}
static t_trie    *construct_local_entry(char *original)
{
    t_trie  *head;
    char    *pwd;

    pwd = NULL;
    head = NULL;
    if (!(pwd = getcwd(pwd, 4096)))
        return NULL;
    head = fill_variant_list(original, pwd, head);
    free(pwd);
    return head;
}

static t_trie    *init_auto_trie(char *original, t_env **env)
{
    t_auto  *arg;
    t_trie  *head;
    t_env    *way;
    int     len;
    char    **ways;
    char    *pwd;
    int     i;

    i = 0;
    pwd = NULL;
    arg = NULL;
    head = NULL;
    way = find_env_variable(env, "PATH");
    ways = ft_strsplit(way->value, ':');
    if (!(pwd = getcwd(pwd, 4096)))
        return NULL;
    head = fill_variant_list(original, pwd, head);
    while (ways[i])
    {
        head = fill_variant_list(original, ways[i], head);
        i++;
    }
    i = 0;
    len = ft_strlen(original);
    char    *bilt;
    while (i <= 6)
    {
        bilt = builtin_list(i);
        if (ft_strnequ(bilt, original, len))
            head = insert_word_trie(head, builtin_list(i));
        i++;
    }
    free(pwd);
    ft_strsplit_free(&ways);
    return head;
}


t_trie      *construct_env_trie(char *orig, t_env **env)
{
    t_env   *curs;
    t_trie  *head;

    head = create_trie_node(0);
    head->asc[4] = create_trie_node('$');
    curs = *env;
    while(curs)
    {
        head->asc[4] = insert_word_trie(head->asc[4], curs->name);
        curs = curs->next;
    }
    return head;
}

char        *split_path(char *orig, char **path)
{
    int     len;
    int     tmp;
    char    *sub;

    
    len = ft_strlen(orig) - 1;
    tmp = len;
    while (len >= 0)
    {
        if (orig[len] == '/')
            break ;
        len--;
    }
    len++;
    if (orig[0] == '/')
        *path = ft_strsub(orig, 0, len);
    else {
        *path = ft_strnew(4096);
        char        *pwd = NULL;
        if (!(pwd = getcwd(pwd, 4096)))
            return NULL;
        ft_strcat(*path, pwd);
        ft_strcat(*path, "/");
        ft_strncat(*path, orig, len - 1);
        ft_strcat(*path, "/");
    }
    sub = ft_strnew(257);
    ft_strcpy(sub, &orig[len]);
    //sub = ft_strdup(&orig[len]);
    return sub;
}

t_trie    *construct_trie(char **orig, t_env **env, int source)
{
    t_trie *head;
    t_env   *curs;
    char    *ptr;
    char    *path;
    char    *sub;
    int     len;
    head = NULL;
    path = NULL;

    if (source == ENV_ONLY) {
        head = construct_env_trie(*orig, env);
    }
    else if (source == EMPTY) {
        orig[0][0] = 0;
        head = fill_variant_list("", ".", head);
    }
    else if (source == GLOBAL) {
        head = init_auto_trie(*orig, env);
    }
    else if (source == LOCAL) {
        ft_memmove(*orig, &(*orig)[2], ft_strlen(*orig) - 1);
        head = construct_local_entry(*orig);
    }
    else if (source == LOC_DIRECTORY)
    {
        
    }
    else if (source == DIRECTORY) {
        len = ft_strlen(*orig);
        if (orig[0][len - 1] == '/')
            head = fill_variant_list("", *orig, head);
        *orig[0] = 0;
    }
    else {
        sub = split_path(*orig, &path);
        head = fill_variant_list(sub, path, head);
        if (*orig)
            free(*orig);
        *orig = sub;
        if (path)
            free(path);
    }
    if (!head)
        return NULL;
    return head;
}