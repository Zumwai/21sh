#include "sh.h"


static int compare_bultin(char *com)
{
    int     i = 0;
    char    *arr[] = {
        "cd",
        "echo",
        "clear",
        "exit",
        "setenv",
        "unsetenv",
        "ppid",
        "pwd",
        "type",
    };
    while (i< 9)
    {
        if (ft_strequ(com, arr[i]))
            return 1;
        i++;
    }
    return 0;
}

int     sh_type(char **com, t_env **env)
{
    char    *path;

    path = NULL;
    if (!com)
        return 1;
    if (!com[1])
        return 1;
    if (compare_bultin(com[1])) {
        ft_printf("%s - is a shell builtin\n", com[1]);
        return 1;
    }
    if ((path = get_path(com[1], env))) {
        ft_printf("%s - is %s\n", com[1], path);
        return 1;
    }
    return 1;
}