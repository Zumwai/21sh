#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct s_trie
{
	bool leaf;
    int  counter;
	char	data;
	struct s_trie *asc[94];
}		t_trie;

typedef struct s_list
{
    char *name;
    struct s_list *next;
}           t_list;

t_trie *create_trie_node(char c) {
    t_trie  *new;
    int     i;

    i = 0;
    if (!(new = (t_trie *)malloc(sizeof(t_trie))))
		printf("BAD");
  //      handle_exit_errors("Malloc returned NULL");
    while (i < 94)
    {
        new->asc[i] = NULL;
        i++;
    }
    new->counter = 0;
    new->leaf = 0;
    new->data = c;
    return new;
}
 
void free_trie_node(t_trie* node) {
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

t_trie    *insert_word_trie(t_trie *head, char *word)
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
        if (value < 0){
				printf("BAD"); exit(1);}
      //      handle_exit_errors("Trie value is negative\n");
        if (!curs->asc[value]) {
            curs->asc[value] = create_trie_node(word[index]);
        }
        curs = curs->asc[value];
		curs->data = word[index];
        curs->counter++;
		index++;
     //   curs->leaf = 0;
    }
    curs->leaf = 1;
    return head;
}

t_list  *create_new_list(char *line)
{
    t_list *new;

    new = (t_list *)malloc(sizeof(t_list));
    new->next = NULL;
    new->name = strdup(line);
    return (new);
}

t_list  *find_last(t_list *head)
{
    t_list *curs;

    curs = head;
    while (curs->next)
    {
            curs = curs->next;
    }
    return curs;
}

void print_trie(t_trie * root, char **av, int index, t_list *var) {

	int i = 0;
    t_list  *curs;

    if (!root)
        return;
    t_trie* temp = root;

		 //   printf("%S\n", *av);
	if (temp->data != -1) {
		av[0][index] = temp->data;
		av[0][index + 1] = '\0';
        index++;
	}
    if (temp->leaf == true)
    {
        t_list *curs = find_last(var);
        curs->next = create_new_list(*av);
    }
   	while (i < 94) {
		if (temp->asc[i])
     	   print_trie(temp->asc[i], av, index, var);
		i++;
    }
}

char    *get_env(char **env)
{
    int i = 0;
    char    *new;
    while (env[i])
    {
        if (!strncmp(env[i], "PATH", 4)) {
            new = strdup(&env[i][5]);
            return new;
        }
        i++;
    }
    return NULL;
}

int main(int ac, char **av, char **env)
{
	t_trie	*test;
    t_list  *head;
	char	*tab;

	int i = 0;
	test = (t_trie *)malloc(sizeof(t_trie));
    char    *value;
    value = get_env(env);
    printf("%s\n", value);
	while (i < 94)
    {
        test->asc[i] = NULL;
        i++;
	}
	tab = (char *)malloc(sizeof(char) * 256);
	test->data = -1;
	bzero(tab, 256);
	test = insert_word_trie(test, "abcdef");
	test = insert_word_trie(test, "abf");
	test = insert_word_trie(test, "def");
	test = insert_word_trie(test, "a");
	test = insert_word_trie(test, "abcdefg");
	test = insert_word_trie(test, "b");
	test = insert_word_trie(test, "c");
    head = (t_list *)malloc(sizeof(t_list));
    head->name = NULL;
    head->next = NULL;
    t_list *curs;
	print_trie(test, &tab, 0, head);
    curs = head->next;
    while (curs)
    {
        puts(curs->name);
        curs = curs->next;
    }
//	printf("%zu\n", sizeof(test));
	return (0);
}
