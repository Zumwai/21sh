/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_libhead.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/22 15:31:28 by aophion           #+#    #+#             */
/*   Updated: 2020/07/28 20:25:50 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>

typedef struct		s_liblist
{
	void			*content;
	size_t			content_size;
	struct s_liblist	*next;
}					t_liblist;

int					ft_atoi(const char *str);
void				ft_bzero(void *s, size_t n);
int					ft_isalnum(int c);
int					ft_isalpha(int c);
int					ft_isascii(int c);
int					ft_isdigit(int c);
int					ft_isprint(int c);
char				*ft_itoa(int n);
void				*ft_memalloc(size_t n);
int					ft_ischar(char c);
void				*ft_memcpy(void *dst, const void *src, size_t n);
void				*ft_memccpy(void *dst, const void *src, int c, size_t n);
void				*ft_memchr(const void *s, int c, size_t n);
int					ft_memcmp(const void *s1, const void *s2, size_t n);
void				ft_memdel(void **ap);
void				*ft_memmove(void *dst, const void *src, size_t len);
void				*ft_memset(void *s, int c, size_t n);
void				ft_putchar(char c);
void				ft_putchar_fd(char c, int fd);
void				ft_putendl(char const *s);
void				ft_putendl_fd(char const *s, int fd);
void				ft_putnbr(int n);
void				ft_putnbr_fd(int n, int fd);
void				ft_putstr(char const *s);
void				ft_putstr_fd(char const *s, int fd);
char				*ft_strcat(char *dest, const char *src);
char				*ft_strchr(const char *s, int c);
void				ft_strclr(char *s);
int					ft_strcmp(const char *s1, const char *s2);
char				*ft_strcpy(char *dst, const char *src);
void				ft_strdel(char **as);
char				*ft_strdup(const char *s1);
int					ft_strequ(const char *s1, const char *s2);
void				ft_striter(char *s, void (*f)(char *));
void				ft_striteri(char *s, void (*f)(unsigned int, char *));
char				*ft_strjoin(char const *s1, char const *s2);
size_t				ft_strlcat(char *dest, const char *src, size_t size);
size_t				ft_strlen(const char *s);
char				*ft_strmap(char const *s, char (*f)(char));
char				*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char				*ft_strncat(char *dest, const char *src, size_t n);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
char				*ft_strncpy(char *dst, const char *src, size_t len);
int					ft_strnequ(char const *s1, char const *s2, size_t n);
char				*ft_strnew(size_t size);
char				*ft_strnstr(const char *haystack,
		const char *needle, size_t len);
char				*ft_strrchr(const char *s, int c);
char				**ft_strsplit(const char *s, char c);
char				*ft_strstr(const char *haystack, const char *needle);
char				*ft_strsub(char const *s, unsigned int start, size_t len);
char				*ft_strtrim(char const *s);
int					ft_tolower(int c);
int					ft_toupper(int c);
void				ft_print_tab(char **tab);
char				**ft_newdim(size_t n);
int					ft_sqrt(int nb);
t_liblist				*ft_lstnew(void const *content, size_t content_size);
void				ft_lstdelone(t_liblist **alst, void (*del)(void *, size_t));
void				ft_lstdel(t_liblist **alst, void(*del)(void *, size_t));
void				ft_lstadd(t_liblist **alst, t_liblist *new);
void				ft_lstiter(t_liblist *lst, void (*f)(t_liblist *elem));
t_liblist				*ft_lstmap(t_liblist *lst, t_liblist *(*f)(t_liblist *elem));
int					get_next_line(int fd, char **line);
int					ft_isletter(char c);
char				**ft_splitword(char *l);
int				ft_printf(const char *ft, ...);
size_t			ft_strsplit_len(char **strsplit);
void	ft_strsplit_free(char ***strsplit);


#endif
