#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

typedef struct s_auto
{
    char *name;
    struct s_auto *next;
}           t_auto;


t_auto  *create_new_list(char *line)
{
    t_auto *new;

    if (!(new = (t_auto *)malloc(sizeof(t_auto))))
        printf("BAD");
       handle_exit_errors("Malloc returned NULL");
    new->next = NULL;
    if (line)
        new->name = strdup(line);
    else
        new->name = NULL;
    return (new);
}


{
    t_auto  *head;
	char	*tab;
    char    *path;

    tab = (char *)malloc(sizeof(char) * 10);
    bzero(tab, 10);
    tab[0] = 'a';
    tab[1] = 'b';
    path = NULL;
    if (!(path = getcwd(path, 4096)))
    { printf("BAD");	    return 0;}
    int i;
    char    *tmp;
    while (strncmp(env[i], "PATH", 4))
        i++;
    head = local_names(tab, path);
    while (head)
    {
        printf("%s\n", head->name);
        head = head->next;
    }
	return (0);
}
