#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct s_trie
{
	bool leaf;
	char	data;
	struct s_trie *asc[94];
}		t_trie;


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
		index++;
     //   curs->leaf = 0;
    }
    curs->leaf = 1;
    return head;
}

void print_trie(t_trie * root, char **av, int index) {
	int i = 0;
    if (!root)
        return;
    t_trie* temp = root;
	if (temp->leaf == true)
			puts(*av);
		 //   printf("%S\n", *av);
	if (temp->data != -1) {
		av[0][index] = temp->data;
		av[0][index + 1] = '\0';
		index++;
	}
   	while (i < 94) {
		if (temp->asc[i])
     	   print_trie(temp->asc[i], av, index);
		i++;
    }
}

int main(void)
{
	t_trie	*test;
	char	*tab;

	int i = 0;
	test = (t_trie *)malloc(sizeof(t_trie));
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

	
	/*
	test = insert_word_trie(test, "12345");
	
	test = insert_word_trie(test, "abc");
	test = insert_word_trie(test, "1ABDE123");
	*/
	print_trie(test, &tab, 0);
//	printf("%zu\n", sizeof(test));
	return (0);
}
