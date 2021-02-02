#include "sh.h"
/* DEBUG GLOBAL */
static size_t	g_size;
static size_t	g_count;
static size_t   g_single;

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
    new->counter = 0;
    new->leaf = 0;
    new->data = c;
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
          if (node->asc[i] != NULL){
         if (node->counter == 1)
               g_single++;
                free_trie_node(node->asc[i]);
          }

        i++;
    }
    g_count++;
    g_size += sizeof(t_trie);
    free(node);
}

static int  convert_asc_value(char c)
{
    /*
    if (c >= 48 && c <= 57)
        return c - 48;
    else if (c >= 65 && c <= 90)
        return c - 65 + 10;
    else if (c >= 97 && c <= 122)
        return c - 97 + 36;
        */
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

    curs = head;
    index = 0;
    value = 0;
    while (word[index])
    {
        value = convert_asc_value(word[index]);
        if (value < 0)
            handle_exit_errors("Trie value is negative!\n");
        if (!curs->asc[value]) {
            curs->asc[value] = create_trie_node(word[index]);
        }
        curs = curs->asc[value];
		curs->data = word[index];
        curs->counter++;
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
    if (line)
        new->name = ft_strdup(line);
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

static void create_names_list(t_trie * root, char **av, int index, t_auto *head) {

	int i = 0;
    t_auto  *curs;

    if (!root)
        return;
    t_trie* temp = root;
	if (temp->data != -1) {
		av[0][index] = temp->data;
		av[0][index + 1] = '\0';
        index++;
	}
    if (temp->leaf == true)
    {
        t_auto *curs = find_last(head);
        curs->next = create_new_list(*av);
    }
   	while (i < 94) {
		if (temp->asc[i])
     	    create_names_list(temp->asc[i], av, index, head);
		i++;
    }
}

t_auto  *fill_variant_list(char *orig, char *path, t_auto *arg)
{
    DIR     *dir;
    struct dirent *container;
    t_auto  *source;
    t_auto  *head;
    int     len;

    len = 0;
    source = find_last(arg);
    dir = opendir(path);
    if (!dir)
        return arg;
    if (!orig)
        return NULL;
    len = ft_strlen(orig);
    while ((container = readdir(dir)))
    {
        if (ft_strnequ(container->d_name, orig, len))
        { 
            if (!source)
            {
                source = create_new_list(container->d_name);
                arg = source;
            }
            else
            {
                source->next = create_new_list(container->d_name);
                source = source->next;
            }
        }
    }
    free(dir);
    return arg;
}

t_trie    *init_auto_trie(char *original, t_env **env)
{
    t_auto  *arg;
    t_trie  *head;
    t_env    *way;
    char    **ways;
    char    *pwd;
    int     i;
    char    *naming;

    i = 0;
    pwd = NULL;
    arg = NULL;
    naming = ft_strnew(256);
    way = find_env_variable(env, "PATH");
    ways = ft_strsplit(way->value, ':');
    if (!(pwd = getcwd(pwd, 4096)))
        return NULL;
    arg = fill_variant_list(original, pwd, arg);
    while (ways[i])
    {
        arg = fill_variant_list(original, ways[i], arg);
        i++;
    }
    t_auto *saved = arg;
    head = create_trie_node(0);
    while (arg)
    {
        head = insert_word_trie(head, arg->name);
        arg = arg->next;
    }
    free(naming);
    free(pwd);
    t_auto *curs;
    arg = saved;
    while (arg)
    {
        curs = arg;
        arg = arg->next;
        free(curs->name);
        free(curs);
    }
    ft_strsplit_free(&ways);
    return head;
}

int  search_word(t_trie *root, char *word)
{
    int     i = 0;
    int     value = 0;
    t_trie  *curs = root;

    while (word[i])
    {
        value = convert_asc_value(word[i]);
        if (!curs->asc[value])
            return 0;
        curs = curs->asc[value];
    }
    if (curs != NULL && curs->leaf == true)
        return 1;
    return 0;
}

char *search_trie(t_trie *root, char *word)
{
    t_trie *curs;
    char    *av;
    int     value;
    int     i;

    curs = root;
    i = 0;
    value = 0;
    if (!search_word(root, word))
        return NULL;
    av = ft_strnew(256);
    ft_strcpy(av, word);
    return av;
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

int	autocomplete(t_term *pos, t_env **env, t_yank *buf)
{
	t_trie *head;
	char	*orig;
	char	*new;

g_single = 0;
	new = NULL;
    g_size = 0;
     g_count = 0;
	if (!(orig = get_incomplete(pos)))
        return 1;
	head = find_best_match(orig, env);
//	new = search_trie(head, orig);
    set_free_null(&orig);
    if (head)
        free_trie_node(head);
            printf("%lu - total number\n", g_count);
    printf("%zu - single\n", g_single);
    printf("%lu  -size\n", g_size);

	return 1;
}