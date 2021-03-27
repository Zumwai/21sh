/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 13:01:55 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 13:02:49 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef AUTO_H
# define AUTO_H
void    print_words(t_trie *node, char **line, int index, t_auto *list);
void    print_varians(t_term *pos, t_auto *list, char *orig);
t_auto  *create_new_list(char *line);
int 	is_relative_path(char *orig);
char	*get_incomplete(t_term *pos, int *cd);
t_trie	*construct_trie(char **orig, t_env **env, int source);
void	free_trie_node(t_trie* node);
int		convert_asc_value(char c);
t_trie	*find_best_match(char *orig, t_env **env);
int		autocomplete(t_term *pos, t_env **env, t_yank *buf);
void	free_trie_node(t_trie* node);
t_trie	*check_existence(t_trie *head, char *orig, char **remainder);
int		get_to_the_diversion(t_trie *node, char **buf, int index);
char	*search_trie(t_trie *head, char *orig, t_auto *list);
extern t_trie *create_trie_node(char c);
extern t_trie **init_array(void);
extern char *builtin_list(int i);
int	check_for_dir(char *orig, char *new);
t_trie	*insert_word_trie(t_trie *head, char *word);
extern char			*concat_pwd(char **path, char *orig, int len);
extern char				*split_path(char *orig, char **path);
extern t_trie		*fill_relative_path(char **orig);
extern t_trie		*fill_local_dir(char **orig, int len, int source);
t_trie		*construct_local_entry(char *original, int flag);
extern void			insert_var_dir(char *path, char *name, t_trie *head);
extern t_trie		*fill_variant_dirs(char *orig, char *path, t_trie *head);
extern t_trie		*fill_env_path(t_trie *head, char *orig,
					t_env **env, char **ways);
extern t_trie *fill_variant_list(char *orig, char *path, t_trie *head);

#endif