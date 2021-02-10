#include "sh.h"

static t_auto  *create_new_list(char *line)
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

char	*get_incomplete(t_term *pos)
{
	int		curr;
	int		end;
	int 	abs = ft_abs(pos->delta_x);
	char	*incomplete;

	if (pos->index == 0 || pos->index - abs <= 0)
		return NULL;
	curr = pos->index - abs;
	end = curr;
	while(curr > 0 && pos->new[curr - 1] == ' ')
		curr--;
	while(curr > 0 && ft_ischar(pos->new[curr - 1]))
		curr--;
	incomplete = ft_strsub(pos->new, curr, end - curr);
	return incomplete;
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
        /*
        t_auto *list;
        list = create_new_list(NULL);
        index = ft_strlen(*buf);
        print_words(node, buf, index - 1, list);
        */
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
    if (curs->sub) //sega with "cd ~"
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
        ft_strcpy(buf, orig);
        index = ft_strlen(buf);
        res = get_to_the_diversion(curs, &buf, index - 1);
        if (ft_strcmp(buf, orig)) {
            ret = ft_strdup(&buf[index]);
        }
        if (res < 0)
            print_words(curs, &buf, ft_strlen(buf) - 1, list);
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

void    print_varians(t_auto *list)
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
int	autocomplete(t_term *pos, t_env **env, t_yank *buf)
{
	char	*orig; 
	char	*new;
    char    *dup;
    t_auto *list;
    int     len = 0;

    new = NULL;
	if (!(orig = get_incomplete(pos)))
        return 1;
   dup =  ft_strdup(orig);
    len = ft_strlen(orig);
    if (orig[0] == '$')
        buf->trie = construct_trie(&orig, env, ENV_ONLY);
    else if (orig[0] == '.' && orig[1] == '/')
        buf->trie = construct_trie(&orig, env, LOCAL);
    else if (orig[len - 1] == '/')
        buf->trie = construct_trie(&orig, env, DIRECTORY);
    else if (is_relative_path(orig))
        buf->trie = construct_trie(&orig, env, 4);
    else 
        buf->trie = construct_trie(&orig, env, GLOBAL);
   // else if (orig[0] == '.')
    if (buf->trie) {
        list = create_new_list(NULL);
    	new = search_trie(buf->trie, orig, list);
        if (list->next)
            print_varians(list->next);
            
        if (new && !new[0]) {
            if (check_for_dir(dup, new))
                  yank_buffer(pos, "/");
        } 
        if (new)
            yank_buffer(pos, new);
        //if(check_for_dir(orig, new))
         //   yank_buffer(pos, "/");
        set_free_null(&new);
        free(list);
        free_trie_node(buf->trie);
        buf->trie = NULL;
    }
    //set_free_null(&orig);
    return 0;
}