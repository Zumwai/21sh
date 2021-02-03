#include "sh.h"
/* DEBUG GLOBAL */
static size_t	g_size;
static size_t	g_count;
static size_t   g_single;
static size_t   g_words;


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
  //\  printf("%lu - tire\n", sizeof(t_trie *));
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
    new->asc = NULL;
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
          if (node->asc && node->asc[i] != NULL){
            if (node->counter == 1)
                g_single++;
            free_trie_node(node->asc[i]);
          }
        i++;
    }

    if (node->asc)
        free(node->asc);
    if (node->sub)
    {
     //   printf("%s\n", node->sub);
        free(node->sub);
    }
    node->asc = NULL;
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
  //  char    *temp;
    t_trie  *curs;

    if (!head)
        head = create_trie_node(0);
 //   temp = ft_strnew(256);
    curs = head;
    index = 0;
    value = 0;
    while (word[index])
    {
        value = convert_asc_value(word[index]);
        if (value < 0)
            handle_exit_errors("Trie value is negative!\n");
        if (!curs->asc) {
            curs->asc = init_array();
        }
        if (!curs->asc[value]) {
            curs->asc[value] = create_trie_node(word[index]);
            curs = curs->asc[value];
            if (word[index + 1]) {
                curs->sub = ft_strdup(&word[index + 1]);
                curs->leaf = 1;
            }
            else {
                curs->data = word[index];
                curs->counter++;
            }
            return head;
        }
        curs = curs->asc[value];
        if (curs->sub)
        {
            if (curs->sub[1])
              insert_word_trie(curs, curs->sub);
            set_free_null(&curs->sub);
        }
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

/*
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
*/

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
        if (ft_strnequ(container->d_name, orig, len)) {
            g_words++;
        head = insert_word_trie(head, container->d_name);
        }
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
/*
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
*/

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


void    print_remainders(t_trie *head)
{

}

/*
char    *parse_vertex(t_trie *head, char buf[256])
{
    t_trie *curs = head;
    char       dbuf[1];
    int     flag = 0;

    while (curs)
    {
        if (curs->counter != 1) {
            print_remainders(curs);
            if (!flag)
                return NULL;
            return ft_strdup(buf);
        }
        if (!curs->sub) {
            dbuf[0] = curs->data;
            ft_strcat(buf, dbuf);
            flag++;
        } else
        {
            ft_strcat(buf, curs->sub);
            flag++;
        }
    }
    return ft_strdup(buf);
}
*/

static void create_names_list(t_trie *root, char **av, int index, t_auto *head) {

	int i = 0;
    t_auto  *curs;

    if (!root)
        return;
    t_trie* temp = root;
	if (temp->data != -1) {
        if (temp->sub)
        {
            ft_strcat(av[0], temp->sub);
            int len = ft_strlen(temp->sub);
            index += len;
            /*
            t_auto *curs = find_last(head);
            curs->next = create_new_list(*av);
            return */
        } else {
	    	av[0][index] = temp->data;
	    	av[0][index + 1] = '\0';
            index++;
        }
	}
    if (temp->leaf == true)
    {
        t_auto *curs = find_last(head);
        curs->next = create_new_list(*av);
    }
   	while (i < 94) {
		if (temp->asc && temp->asc[i]) {
            create_names_list(temp->asc[i], av, index, head);
        }
		i++;
    }
}


char    *search_trie(t_trie *head, char *orig)
{
    t_trie  *curs;
    int     flag = 0;
    int     i = 0;
    int     value;
     char    *buf;
    char    dbuf[1];
    char    *ret;
    t_auto  *root;

    ret = NULL;
   buf = ft_strnew(256);
    curs = head;
    while(orig[i])
    {
        value = convert_asc_value(orig[i]);
        if (!curs->asc || !curs->asc[value])
            return NULL;
        curs = curs->asc[value];
        i++;
    }
    ft_strcpy(buf, orig);
    int index = ft_strlen(buf);
    root = NULL;
    root = (t_auto *)malloc(sizeof(t_auto));
    root->next = NULL;
    create_names_list(curs, &buf, index + 1, root);
    int j = 0;
    while (root)
    {
        printf("%s\n", root->name);
        root = root->next;
        j++;
    }
    printf("%d - number\n", j);
   // ret = parse_vertex(curs, buf);
    return ret;
}

int	autocomplete(t_term *pos, t_env **env, t_yank *buf)
{
	t_trie *head;
	char	*orig;
	char	*new;

    g_single = 0;
    g_words = 0;
	new = NULL;
    g_size = 0;
    g_count = 0;
	if (!(orig = get_incomplete(pos)))
        return 1;
	head = find_best_match(orig, env);
     if (head)
    	new = search_trie(head, orig);
    set_free_null(&orig);
    if (head)
        free_trie_node(head);
    printf("%lu -size\n", g_size);
    printf("%lu - total number\n", g_count);
    printf("%zu - single\n", g_single);
    printf("%lu - words\n", g_words);
	return 1;
}