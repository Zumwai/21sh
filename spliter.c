#include "libft/libft.h"

static int		ft_word_len(char const *s, char c)
{
	int let;

	let = 0;
	while (*s && *s != c)
	{
		let++;
		s++;
	}
	return (let);
}

static int		ft_count_words(char const *s, char c)
{
	int i;
	int word;

	i = 0;
	word = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		else
		{
			word++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (word);
}

static void		ft_free(char **s, int i)
{
	while (i--)
		ft_strdel(&(s[i]));
	free(*s);
}

static char		**ft_create_word(const char *s, char c, int nb, char **arr)
{
	int			i;
	int			j;

	i = 0;
	j = 0;
	while (i < nb)
	{
		if (s[j] && s[j] != c)
		{
			arr[i] = ft_strsub(s, j, ft_word_len(&s[j], c));
			if (arr[i])
			{
				i++;
				j = j + (ft_word_len(&s[j], c));
			}
			else
			{
				ft_free(arr, i);
				return (NULL);
			}
		}
		j++;
	}
	arr[nb] = NULL;
	return ((char **)arr);
}

char			**ft_strsplit(char const *s, char c)
{
	char		**arr;
	int			word_nb;
	int			i;
	int			j;

	j = 0;
	i = 0;
	if (s == NULL)
		return (NULL);
	word_nb = ft_count_words(s, c);
	arr = (char **)malloc(sizeof(char *) * (word_nb + 1));
	if (arr == NULL)
		return (NULL);
	ft_create_word(s, c, word_nb, arr);
	return (arr);
}

int main(void)
{
    char    *a;

    a = ft_strnew(2);
    a[0] = '/';
    char    **split = NULL;
    split = ft_strsplit(a, '/');
    return (0);
}