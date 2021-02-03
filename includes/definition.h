#ifndef DEFINITION_H
# define DEFINITION_H

# define LEFT	4479771			//left arrow
# define RIGHT	4414235			//right
# define ENTER	10				//return
# define BACKSPACE 127			//backspace
# define DELETE	2117294875		//delete
# define START	4741915			//home
# define END	4610843			//end
# define L_WORD 74995417045787	//ctrl + left arrow
# define R_WORD 73895905418011	//+ right
# define L_UP	71696882162459	//+ up
# define L_DOWN	72796393790235	//+ down
# define R_CUT	11				//ctrl + k
# define L_CUT	21				//ctrl + u
# define YANK	25				//ctrl + y
# define W_CUT	23				//ctrl + w
# define CLEAR	12				//ctrl + l
# define HISTORY_UP	4283163		//arrow up
# define HISTORY_DOWN	4348699 //arrow down
# define COPY_W	15				//ctrl + o
# define TAB 	9				//tab or ctrl + i, unfortunately
# define BACK_TAB	5921563		//backtab

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
typedef struct s_trie
{
	bool			leaf;
    int				counter;
	char			data;
	char			*sub;
	struct s_trie **asc;
}		t_trie;

typedef struct	s_inherit
{
	bool		leaf;
	int			counter;
	char		*data;
	int			wrong;
}				t_inherit;

typedef struct s_auto
{
    char *name;
    struct s_auto *next;
}           t_auto;

typedef struct 		s_term
{
	int				x;
	int				y;
	int				delta_x;
	int				delta_y;
	int				index;
	int				prompt; //dont need yet
	int				buf_size;
	int				state;
	int				heredoc;
	char			*substr;
	char			*new;
	struct s_term	*next;
	struct s_term	*prev;
}					t_term;

typedef struct 		s_history
{
	t_term				*line;	
	struct s_history	*next;
	struct s_history	*prev;
}					t_history;

typedef struct s_yank
{
	char			*yanked;
	int				size;
	int				counter;
	char			*sub;
	t_history		*hist_ptr;
	t_term			*input;
	t_term			*current;
	t_term			*saved;
	t_history		*history;
	t_auto			*completion;
}					t_yank;

enum				e_state
{
	DEFAULT,
	QUOTES,
	DOUBLE_QUOTES,
	HEREDOC,
	POST_DOC
};

#endif