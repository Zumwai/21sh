#include "sh.h"

int    get_to_the_diversion(t_trie *node, char **buf, int index)
{
    int i;
    int ret = 0;

    i = 0;
    if (!node)
        return -1;
    //if (!buf[0][0])
    //    return 0;
    buf[0][index] = 0;
    if (node->sub)
    {
        index += ft_strlen(node->sub);
        ft_strcat(*buf, node->sub);
        buf[0][index] = '\0';
        //node->sub[index] = '\0';
        return 0;
    }
    buf[0][index] = node->data;
    buf[0][index + 1] = 0;
    index++;
    if (node->leaf)
        return -1;
    if (node->counter > 1 || node->leaf == true)
    {  
        return -2;
    }
    while (i < 94)
    {
        if (node->asc[i])
            ret = get_to_the_diversion(node->asc[i], buf, index);
        if (ret == -1 || ret == -2)
            return ret;
        i++;
    }
    return 0;
}

t_trie  *check_existence(t_trie *head, char *orig, char **remainder)
{
    int value = 0;
    int ret = 0;
    int     size = 0;
    int i = 0;
    t_trie *curs;

    curs = head;
    while (orig[i])
    {
       value = convert_asc_value(orig[i]);
        if (!curs->sub && !curs->asc[value])
            return NULL;
        if (curs->sub) {
            size = ft_strlen(orig);
            ret = ft_strncmp(curs->sub, &orig[i - 1], size - i + 1);
            if (!ret) {
                ft_strcpy(*remainder, &curs->sub[size - i + 1]);
                return curs;
            }
            return NULL;
        }
        curs = curs->asc[value];
        i++;
    }
    if (curs->sub)
        ft_strcpy(*remainder, &curs->sub[1]);
    return curs;
}

char  *search_trie(t_trie *head, char *orig, t_auto *list)
{
    t_trie  *curs;
    int     j = 0;
    int     i = 0;
     char    *buf;
    char     *comp;
    char    *ret = NULL;
    int     index = 0;
    int     res = 0;


    comp = ft_strnew(257);
    curs = check_existence(head, orig, &comp);
    if (!curs)
        return NULL;
    if (!curs->sub) {
        buf = ft_strnew(257);
        strcat(buf, orig);
        index = ft_strlen(buf);
        res = get_to_the_diversion(curs, &buf, index);
        if (ft_strcmp(buf, orig)) {
            ret = ft_strdup(&buf[index]);
        }
        if (res < 0) {
            if (res == -1) {
                ft_strclr(buf);
                ft_strcpy(buf, orig);
            }
            print_words(curs, &buf, 0, list);
        }
        free(buf);
        free(comp);
        return ret;
    }
    else {
        buf = ft_strnew(257);
        ft_strcpy(buf, comp);
        free(comp);
        return buf;
    }
    return NULL;
}

int     check_for_dir(char *orig, char *new)
{
    char    path[PATH_MAX] = {0};

	struct stat		per;
	unsigned int	tmp;
    int             len = 0;
    if (orig[0] == '/')
    {
        ft_strcat(path, orig);
        if (new)
            ft_strcat(path, new);
    } else {
        char	*curr = NULL;

	    if (!(curr = getcwd(curr, PATH_MAX))) {
	    	handle_exit_errors("GETCWD got PWND");
        }
        ft_strcat(path, curr);
        ft_strcat(path, "/");
        ft_strcat(path, orig);
        free(curr);
        if (new)
            ft_strcat(path, new);
    }
    if ((lstat(path, &per)) == -1)
		    return (0);
    tmp = (per.st_mode & S_IFMT);
    if (S_ISDIR(tmp)) {
        len = ft_strlen(path);
        if (path[len - 1] == '/')
            return 0;
	    return (1);
    }
    return 0;
}

int	autocomplete(t_term *pos, t_env **env, t_yank *buf)
{
	char	*orig; 
	char	*new;
    char    *path;
    char    *dup;
    t_auto *list;
    t_env   *curs;
    char    *ptr;
        int     len = 0;
    int     cd = 0;

    //if (pos->state || pos->heredoc)
     //   return 1;
    new = NULL;
	if (!(orig = get_incomplete(pos, &cd)))
        return 1;
    dup =  ft_strdup(orig);
    len = ft_strlen(orig);

    /*ENV*/
    if ('~' == orig[0] ) {
        curs = find_env_variable(env, "HOME");
        if (curs && curs->value)
        {
            path = ft_strnew(PATH_MAX);
            ft_strcpy(path, curs->value);
            if (orig[1]) {
                ptr = ft_strchr(orig, '~');
                ft_strcat(path, ptr + 1);
            }
            free(orig);
            orig = path;
            len = ft_strlen(orig);
            free(dup);
            dup = ft_strdup(orig);
        }
    }
    if (orig[0] == '$')
        buf->trie = construct_trie(&orig, env, ENV_ONLY);
    else if (orig[0] == '.' && orig[1] == '/')
        buf->trie = construct_trie(&orig, env, LOCAL);
    else if (len > 0 && orig[len - 1] == '/')
        buf->trie = construct_trie(&orig, env, DIRECTORY);
    else if (is_relative_path(orig))
        buf->trie = construct_trie(&orig, env, RELATIVE);
    else  if (!cd)
        buf->trie = construct_trie(&orig, env, GLOBAL);
    else
        buf->trie = construct_trie(&orig, env, SECOND);
   // else
    //    buf->trie = construct_trie(&orig, env, )
    if (buf->trie) {
        list = create_new_list(NULL);
    	new = search_trie(buf->trie, orig, list);
        if (list->next)
            print_varians(pos, list->next);
        if (new && !new[0]) {
            if (check_for_dir(dup, new))
                  yank_buffer(pos, "/");
        } 
        if (new)
            yank_buffer(pos, new);
        set_free_null(&new);
        free(list);
        free_trie_node(buf->trie);
        buf->trie = NULL;
    }
    else {
        if (check_for_dir(orig, ""))
        yank_buffer(pos, "/");
    }
    free_trie_node(buf->trie);
    set_free_null(&dup);
    set_free_null(&orig);
    return 0;
}

