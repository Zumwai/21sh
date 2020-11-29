#include "libft.h"

int		ft_ischar(char c)
{
//	if (c == ' ' || c == '\t')
//		return 0;
//	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
//		return 1;
	if (c > 32 && c < 127)
		return 1;
	return 0;
}