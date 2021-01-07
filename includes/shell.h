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

#ifndef SHELL_H
# define SHELL_H

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
# include <termios.h>
# include <termcap.h>
# include <fcntl.h>
# include "control.h"
# include <stdbool.h>

# define HOME	("HOME")
# define OLDPWD ("OLDPWD")
# define PWD	("PWD")
# define FIN	(-42)		//return code for program termination. For personal challenge of not using exit in minishell
# define LSTA	(-2)	//lstat returned error
# define EXIS	(-3)	//doesnt exist
# define IXUS	(-4)	//cant be executed by owner
# define NOEX	(-6)	//couldnt be executed(dir for example)
# define NODIR	(-7)	//not a dir

typedef struct	s_env
{
	char		*name;
	char		*value;
	struct s_env *next;
}				t_env;


/*
	BUILTINS
*/
int				ft_cd(char **com, t_env **ev);
int				ft_pwd(void);
int				ft_echo(char **av, t_env **ev);
int				ft_exit(void);
int				ft_help(void);
void			display_id_kid_parent(void);
int				manage_env(char **com, t_env **ev);
int				set_env(char **com, t_env **ev);
int				unset_env(char **com, t_env **ev);
t_env			*add_env(char *com);
t_env			*create_env_list(char **env);
void			delete_env_list(t_env **ev);
t_env			*find_env(t_env **ev, char *name);
int				display_env_list(char **com, t_env **env);
t_env			*ft_last_env(char *name, char *value);
t_env			*init_shell(int ac, char **av, char **env, t_yank **buffer);

/*
	READER, the old one
*/

int				register_input(t_env **ev, char *line);




/*
	CMD
*/
int				go_do(char **com, t_env **env);
int				create_child_cmd(t_env **ev, char **av, char *path);
int				check_rights(char *path, int cd);

/*
	SIGNALS
*/
void			handle_main_signal(int sig);
void			handle_child_signal(int sig);
void			handle_all_signal(int pid);

/*
	UTILS
*/
void			ft_concat(char *str, char **path, char *name);
char			*get_full_path(char *path, char *name);
void			handle_empty_error(char *name, char *description);
void			handle_cd_err(int num, char *name);
int				handle_return_error(int num, char *name);
void			ft_free_tab(char **tab);
void			handle_exit_errors(char *description);
int				putchar_like(int n);
int 			ft_abs(int i);
void 			ft_putstr_size(char *line, size_t size);
void 			set_free_null(char **line);
void			set_free_all(t_env *ev, t_yank *buffer);
char			*concat_lines(t_term *input);



#endif
