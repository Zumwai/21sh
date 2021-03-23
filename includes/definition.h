#ifndef DEFINITION_H
# define DEFINITION_H

#ifdef __APPLE__ 
/*todo macos keys */
#else
#ifndef MAX_ARG_STRLEN
# define MAX_ARG_STRLEN 131072
#endif

#ifndef MAX_ARG_STRINGS 
# define MAX_ARG_STRINGS 0x7FFFFFFF
#endif

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
#endif

# define HIST_UP -3
# define HIST_D -4
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

# define ENV_ONLY 0
# define GLOBAL  1
# define LOCAL   2
# define DIRECTORY 3
# define RELATIVE 4
# define HOMEDIR 6
# define EMPTY	7
# define LOC_DIRECTORY 8
# define LOC_FINISH 9
# define SECOND 10
# define LOGICAL 1
# define PHYSICAL 2
# define PREVIOUS 3

typedef struct	s_env
{
	int			scope;
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
	in = 8,
	heredoc = 9,
}			t_type_cmd;

typedef struct			s_cmd
{
	char				**arr; //аргументы
	int					num_of_redir;
	int					*fd1; // для перенаправлений с конкретного на конкретный дескриптор
	int					*fd2;
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
	struct s_trie *asc[95];
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
	int	 size;
    struct s_auto *next;
}           t_auto;

typedef struct s_scroll
{
	int		size;
	char	**arr;
}				t_scroll;

typedef struct s_hdoc {
	int				cord;
	bool			used;
	int				quotes;
	char			*eot;
	struct s_hdoc	*next;
}			t_hdoc;

typedef struct		s_actual
{
	char			*line;
	int				state;
	int				state_before;
	size_t			size;
	struct s_hdoc	*hdoc;
}					t_actual;

typedef struct 		s_term
{
	int				x;
	int				y;
	int				delta_x;
	int				delta_y;
	int				index;
	int				buf_size;
	char			*new;
	struct s_actual *main;
	struct s_scroll	*store;
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
	int				win_x;
	int				win_y;
	int				winch;
	int				diff;
	t_history		*hist_ptr;
	t_term			*input;
	t_term			*current;
	t_term			*saved;
	t_history		*history;
	t_auto			*completion;
	t_trie			*trie;
	char			*actual;
}					t_yank;

typedef struct	s_shlist
{
	t_env	**env;
	int		fd[1024];
	char	*line;
}				t_shlist;

typedef struct s_control
{
	unsigned int	winch;
	struct termios	old;
	struct termios	work;
}				t_control;

# define	DEFAULT		0
# define	QUOTE		1<<0
# define	D_QUOTE		1<<1
# define	GLUE		1<<2
# define	REQ_HDOC	1<<3
# define	HEREDOC		1<<4
# define	ARG_HDOC	1<<5
# define	READ_HDOC	1<<6
# define	FAILED		1<<7

/*
enum				e_state
{
	DEFAULT,
	QUOTES,
	DOUBLE_QUOTES,
	HEREDOC,
	POST_DOC
};
*/
enum				e_glue
{
	NONE,
	BEG,
	MID,
	FIN
};

#endif