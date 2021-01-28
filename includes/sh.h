#ifndef SH_H
#define SH_H

# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include "libft.h"
# include "control.h"
# include <termios.h>
# include <termcap.h>


# include <unistd.h>
# include <limits.h>
//# include <linux/limits.h>		//temporary solution for some macros in linux
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


# define BK "&"
# define AND "&&"
# define OR "||"
# define PIPE "|"
# define SC ";"
# define GREAT ">"
# define GGREAT ">>"
# define LESS "<"
# define LLESS "<<"

# define CD "cd"
# define SETENV "setenv"
# define UNSETENV "unsetenv"
# define ENV "env"
# define PWD "pwd"
# define EXIT "exit"

# define LSTA	(-2)	//lstat returned error
# define EXIS	(-3)	//doesnt exist
# define IXUS	(-4)	//cant be executed by owner
# define NOEX	(-6)	//couldnt be executed(dir for example)
# define NODIR	(-7)	//not a dir

# define PATH_MAX 4096

typedef struct	s_env
{
	char		*name;
	char		*value;
	struct s_env *next;
}				t_env;

typedef enum {
	cmd = 1,
	ext = 2,
}				t_type_token;

typedef enum {
	sc = 1,
	pi = 2,
	and = 3,
	ro = 4,
	bk = 5,
	out_w = 6,
	out_add = 7,
}			t_type_cmd;

typedef struct			s_cmd
{
	char				**arr; //аргументы
	int					fd1; // для перенаправлений с конкретного на конкретный дескриптор
	int					fd2;
	int					r_type; // тип редиректа для редиректа
	int					type; //
	char				*target; // имя файла в случае редиректа
	struct s_cmd		*next;
	struct s_cmd		*prev;
}						t_cmd;

typedef struct 			s_flag
{
	int 				d_quot; /// " кроме $, '', '\'
	int 				u_quot; /// ' отменяет все спецсимволами
}						t_flag;

typedef struct 			s_tree
{
	struct s_token		*token;
	int 				type;
	struct s_tree		*parent;
	struct s_tree		*left;
	struct s_tree		*right;
}						t_tree;

typedef struct			s_token
{
	char				*data;
	int				priority;
	int				type;
	int					c_type;
	int					status;
	struct s_token		*next;
	struct s_token		*prev;
}						t_token;

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
int				sh_setenv(char **cmd, t_env **env);
t_env			*sh_unset(char **nm, t_env **env);
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
