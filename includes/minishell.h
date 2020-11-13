/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: student <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/21 16:36:55 by student           #+#    #+#             */
/*   Updated: 2020/07/30 11:38:10 by student          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <limits.h>
# include <linux/limits.h>		//temporary solution for some macros in linux
# include <dirent.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <stdio.h>
# include "libft.h"
# include <signal.h>

# define HOME	("HOME")
# define OLDPWD ("OLDPWD")
# define PWD	("PWD")
# define FIN	(0)		//return code for program tewrmination, shoud be in a structure, d-uh(previous value was -42(in case for an error))
# define LSTA	(-2)	//lstat returned error
# define EXIS	(-3)	//doesnt exist
# define IXUS	(-4)	//cant execute by owner
# define NOEX	(-6)	//couldnt be executed(dir for example)
# define NODIR	(-7)	//not a dir

typedef struct	s_env
{
	char		*name;
	char		*value;
	struct s_env *next;
}				t_env;

int				ft_reader(t_env **ev);
int				go_do(char **com, t_env **env);
int				ft_pwd(void);
int				ft_echo(char **av, t_env **ev);
int				display_env_list(char **com, t_env **env);
int				unset_env(char **com, t_env **ev);
t_env			*add_env(char *com);
int				set_env(char **com, t_env **ev);
int				ft_cd(char **com, t_env **ev);
void			handle_main_signal(int sig);
void			handle_child_signal(int sig);
void			handle_all_signal(int pid);
int				create_child_cmd(t_env **ev, char **av, char *path);

int				check_rights(char *path, int cd);
int				ft_exit(void);
void			display_id_kid_parent(void);
int				manage_env(char **com, t_env **ev);
t_env			*ft_last_env(char *name, char *value);
void			ft_free_tab(char **tab);
t_env			*find_env(t_env **ev, char *name);
void			delete_env_list(t_env **ev);
t_env			*create_env_list(char **env);
int				ft_help(void);
void			ft_concat(char *str, char **path, char *name);
char			*get_full_path(char *path, char *name);
void			handle_empty_error(char *name, char *description);
int				handle_cd_err(int num, char *name);
int				handle_return_error(int num, char *name);

#endif
