#include "sh.h"
/* DEBUG GLOBAL */
static size_t	g_size;
static size_t	g_count;
static size_t   g_single;
static size_t   g_words;
static size_t   g_print;

static t_trie **init_array(void)
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
     g_size += (sizeof(t_trie *) * 94);
     return node;
}

static t_trie *create_trie_node(char c) {
    t_trie  *new;
    int     i;

    i = 0;
    if (!(new = (t_trie *)malloc(sizeof(t_trie))))
        handle_exit_errors("Malloc returned NULL");
    new->counter = 0;
    new->leaf = 0;
    new->data = c;
//    new->asc = NULL;
    
    while (i < 94)
    {
        new->asc[i]= NULL;
        i++;
    }
    
    new->sub = NULL;
    g_size += sizeof(t_trie);
    g_count++;
    return new;
}
 
void free_trie_node(t_trie* node) {
    int     i;

    i = 0;
    if (!node)
        return ;
    while (i < 94)
    {
        //if (node->asc && node->asc[i] != NULL){
        if (node->asc[i] != NULL){
            if (node->counter == 1)
                g_single++;
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

static int  convert_asc_value(char c)
{
    if (c >= 32 && c <= 127)
        return c - 32;
    else
        return -1;    
}

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
 //       if (ft_strnequ(container->d_name, orig, len)) {
  //          g_words++;
        head = insert_word_trie(head, container->d_name);
  //      }
    }
    free(dir);
    return head;
}

static t_trie    *init_auto_trie(char *original, t_env **env)
{
    t_auto  *arg;
    t_trie  *head;
    t_env    *way;
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
    free(pwd);
    ft_strsplit_free(&ways);
    return head;
}

t_trie    *find_best_match(char *orig, t_env **env)
{
    t_trie *head;

    head = init_auto_trie(orig, env);
    if (!head)
        return NULL;
    return head;
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
        g_print++;
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
        if (ret == -1)
            return -1;
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
    char    *ret;
    int     index = 0;

    ret = NULL;
    comp = ft_strnew(257);
    curs = check_existence(head, orig, &comp);
    if (!curs){
        return NULL;
    }

    int     res = 0;
    if (!curs->sub) {
        buf = ft_strnew(257);
        ft_strcpy(buf, orig);
        index = ft_strlen(buf);
        res = get_to_the_diversion(curs, &buf, index - 1);
        if (ft_strcmp(buf, orig)) {
            ret = ft_strdup(&buf[index]);
        }
        if (res == -2)
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
  //      print_words(curs, &buf, index - 1, list);
}
//REWRITE

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
    curs = list;
    cols = dimensions.ws_col / max;
    int     i = 0;
    ft_printf("\n");
    while (curs)
    {
        ft_printf("%s ", curs->name);
        while (curs->size++ < max)
            ft_printf(" ");
        i++;
        if (i == cols) {
            ft_printf("\n"); i = 0;
        }
        curs = curs->next;
    }
    ft_printf("\n");
}
int	autocomplete(t_term *pos, t_env **env, t_yank *buf)
{
	t_trie *head;
	char	*orig;
	char	*new;

    g_single = 0;
    g_words = 0;
    g_size = 0;
    g_count = 0;
    g_print = 0;
    new = NULL;
	if (!(orig = get_incomplete(pos)))
        return 1;
    if (!buf->trie)
        buf->trie = find_best_match(orig, env);
    t_auto *list;
    list = create_new_list(NULL);
    if (buf->trie)
    	new = search_trie(buf->trie, orig, list);
    t_auto *curs;
    curs = list->next;  
    if (curs)
       print_varians(curs);
    if (!new)
        handle_empty_error("TEMP", "autocomplete failed");
    set_free_null(&orig);
    if (new)
        yank_buffer(pos, new);
    set_free_null(&new);
    /*
    if (head)
        free_trie_node(head);
    */
    if (!new)
        return 1;
    return 0;
    /*
    printf("\n");
    printf("%lu -size\n", g_size);
    printf("%lu - total number\n", g_count);
    printf("%zu - single\n", g_single);
    printf("%lu - words\n", g_words);
    printf("%lu - printed\n", g_print);
    */
   
	return 1;
}