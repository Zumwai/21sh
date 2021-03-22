#include "libft/libft.h"
#include <stdio.h>

int	cha_dir(char s[])
{
	char	*new = ft_strdup(s);
	char	*pwd = NULL;
	int	ret;

	ret = 0;

	ret = chdir(new);
	pwd = getcwd(pwd, 4096);
	printf("%s\n", pwd);
	return ret;
}

int main(void)
{
    char    a[] = "includes/../../../../../../../";
	int	ret = cha_dir(a);

	printf("%d\n", ret);
    return (0);
}