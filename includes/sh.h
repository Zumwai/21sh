#ifndef SH_H
#define SH_H

# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <termios.h>
# include <termcap.h>
# include <limits.h>
//# include <linux/limits.h>		//temporary solution for some macros in linux
# include <dirent.h>
# include <string.h>
# include <stdio.h>
# include <signal.h>
# include <fcntl.h>
# include <stdbool.h>
# include <sys/ioctl.h>
# include "control.h"
# include "definition.h"
# include "libft.h"


t_flag				*init_flag(void);
t_flag 				*reset_flag(t_flag *flag);
t_token 			*init_token(void);
t_token 			*put_data_in_new_token(char *line, int *car, t_token *token);
void				update_flag(t_flag *flag, char s);
int 				semantica(t_flag *flag, char *line, int *i, int *j);
int 				is_tokens_true(t_token *t);
t_tree				*get_tree(t_token *token);

/* EXEC */
int			execute(t_cmd *t, t_env **env);
char				*get_path(char *s, t_env **env);

void 				*init_tree(void);
// static -> void 				*set_child(t_tree *child, t_tree *parent, int f);
t_tree				*get_tree(t_token *token);
t_token 			*get_last_token(t_token *t);

char 			*if_ampersand(char *line, int *i, char *res, int j);
char 			*if_vertical_bar(char *line, int *i, char *res, int j);
char 			*get_semantica_ret(char *line, int *i, char *res, int j);
t_tree			*get_tree(t_token *token);
// static -> t_cmd			*get_data_cmd(t_token *t, t_cmd *cmd, t_env *env);
t_cmd			*new_cmd(t_cmd *prev);
t_cmd			*init_cmd(void);
t_cmd			*get_cmd(t_token *t, t_env **env);
t_cmd			*get_data_with_redirect(char *s, t_cmd *cmd);
char			*get_redirect(char *line, int *i, char *res, int j);
void			free_token_list(t_token **t);
void			free_cmd_list(t_cmd **cmd);
t_env			*env_list(void);
/* BUILTIN */

int				sh_clear(__attribute__((unused))char **com, __attribute__((unused))t_env **env);
int				do_builtin(t_cmd *cmd, t_env **env);
int				sh_env(char **com, t_env **env, __attribute((unused))int fd);
int				sh_echo(char **com, t_env **env, int fd);
// static -> t_env			*clear_list(t_env *env);
extern int			sh_setenv(char **nm, t_env **env, __attribute((unused))int fd);
extern t_env				*sh_unset(char **nm, t_env **env, __attribute((unused))int fd);
int				sh_cd(char **cmd, t_env **env);
t_token 		*parsing_t(char *line);
int				sh_setnew(char *nm, char *value, t_env **env);
int				sh_exit(void);
int				display_id_kid_parent(void);
int				sh_pwd(void);
int				check_rights(char *path, int cd);

/* INIT */
t_env			*init_shell(int ac, char **av, char **env, t_yank **buffer);
static t_yank	*init_buffer(void);
static void		init_tty(void);
int				display_env_list(char **com, t_env **ev);

/* FREE */
void set_free_null(char **line);
void	set_free_all(t_env *ev, t_yank *buffer);
void	ft_free_tab(char ***tab);

/* UTILS */
int ft_abs(int i);
t_env	*find_env_variable(t_env **env, char *sought);
void	ft_concat(char *str, char **path, char *name);
char	*get_full_path(char *path, char *name);

#endif
