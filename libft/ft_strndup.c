#include "libft.h"

char    *ft_strndup(const char *src, const int size)
{
    char    *new;

    new = NULL;
    if (!src)
        return NULL;
    if (!(new = ft_strnew(size + 1)))
        return NULL;
    ft_strncpy(new, src, size);
    return new;
}