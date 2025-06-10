#ifndef EXE_H
#define EXE_H           // close, isatty, ttyname, ttyslot, getcwd, chdir,
#include <unistd.h>     // fork, execve, pipe, dup, dup2, write, access
#include <stdlib.h>     // malloc, free, exit
#include <stdio.h>      // perror
#include <fcntl.h>      // open
#include <sys/wait.h>   // wait, waitpid, wait3, wait4
#include <string.h>     // strerror
#include <errno.h>      // errno

#define PATH_MAX 4096

// Redirection types (devono essere coordinati col parser)
typedef enum e_redir_type {
    R_IN,       // <
    R_OUT,      // >
    R_APPEND,   // >>
    R_HEREDOC   // <<
} t_redir_type;

// Struttura per i redirection (dev'essere identica a quella del parser)
typedef struct s_redir {
    int type;              // R_OUT, R_IN, R_APPEND, R_HEREDOC
    char *file;            // usato da open()
    int fd;                // red->fd opzionale, solo se già aperto
    struct s_redir *next;
} t_redir;

// Struttura nodo comando: una pipe = un nodo (da rinominare in argv per compatibilità)
typedef struct s_node {
    char **cmd_argv;       // comando + argomenti (ex. {"ls", "-l", NULL})
    t_redir *red;          // lista di redirect da eseguire
    struct s_node *next;   // nodo successivo nella pipe
} t_node;

// Variabili di ambiente come lista linkata
typedef struct s_env {
    char *key;
    char *value;
    int exported; // 1 = passa a execve() e mostra in env. 0 = visibile nella shell ($VAR), ma non nelle child
    struct s_env *next;
} t_env;

// Builtin identifiers
enum e_btin { CMD_ECHO, CMD_CD, CMD_PWD };

// === exe.c === //

int		exe_pipeline(t_node *head, t_env **env);
int		exe_single_cmd(t_node *node, t_env **env);
int		exec_bin(char **argv, t_env **env);
int		exec_builtin(char **argv, t_env **env);
char 	*join_path(char *dir, char *cmd);
char 	*get_env_value(char *key, t_env *env);
void	free_split_all(char **arr);
int		apply_redirects(t_redir *red);
char 	**env_to_array(t_env *env);
t_env   *init_env_from_system(char **envp);
t_env	*init_basic_env(void);
char *resolve_path(char *cmd, t_env *env);


// === btin.c === //

void	set_env_var(t_env **env, const char *key, const char *value);
int		exe_cd(char **args, t_env **env);
int 	exe_pwd(char **args, t_env **env);
int		exe_env(char **args, t_env **env);
int		exe_echo(char **args);
int		exe_exit(char **args);
int		exe_export(char **args, t_env **env);
int		exe_unset(char **args, t_env **env);


int is_parent_builtin(char *cmd);
int is_child_builtin(char *cmd);
int should_execute_in_parent(t_node *node);
int		is_builtin(char *cmd);



// === utils.c === //

// --- fd 
int		switch_fd(int from, int to);
int		save_fd(int fd);
int		save_stdout(void);
int		open_outfile(const char *filename);

// --- memory 
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t n_elem, size_t size);
void	*safe_alloc(size_t n_elem, size_t size, const char *desc);

// --- libfx 
char	*ft_strchr(const char *s, int c);
char	*ft_strjoin(const char *s1, const char *s2);
char	*ft_strdup(const char *s);
char	**ft_split(const char *s, char c);
size_t	ft_strlen(const char *s);
void	free_array(char **arr);
int		env_size(t_env *env);



/// nodes.c ///

t_node  *create_test_nodes(void);
void    free_test_nodes(t_node *head);


t_node	*node_create(char **argv, t_redir *redirs);
void	node_free(t_node *node);
void	redir_free(t_redir *redir);
void	env_free(t_env *env);
t_env	*env_find(t_env *env, const char *key);



/// debug.c ///

void	debug_status(int status);
void	debug_pipe_test(t_node *node);
void	debug_redirect_info(t_redir *redir);
void	debug_print_node(t_node *node);
void	debug_print_pipeline(t_node *head);
void	debug_print_env(t_env *env);
void	debug_print_array(char **arr);
void	debug_execve_args(char *path, char **argv, char **env);
void	debug_path_resolution(char *cmd, t_env *env);




#endif


/* shared structures



#ifndef MINISHELL_H
# define MINISHELL_H

─────────────────────────────────────────────────────────────
    PARSER STRUCTURES
─────────────────────────────────────────────────────────────

typedef enum e_token_type 
{
	TK_WORD = 0,        // parola generica (comando, arg, opzione)
	TK_PIPE,            // |
	TK_REDIR_IN,        // <
	TK_REDIR_OUT,       // >
	TK_REDIR_APPEND,    // >>
	TK_HEREDOC,         // <<
	TK_S_QUOTE,         // ' (delimitatore opzionale)
	TK_D_QUOTE,         // " (delimitatore opzionale)
	TK_DOLLAR           // $VAR o $
}	t_token_type;

typedef struct s_token 
{
	t_token_type type;
	char         *value;
	struct s_token *next;
}	t_token;



─────────────────────────────────────────────────────────────
    SHARED STRUCTURES
─────────────────────────────────────────────────────────────

typedef enum e_redir_type
{
	R_IN,       // <
	R_OUT,      // >
	R_APPEND,   // >>
	R_HEREDOC   // <<
}	t_redir_type;

typedef struct s_redir 
{
	t_redir_type  type;    // R_IN, R_OUT, etc.
	char          *file;   // nome del file (da open())
	int           fd;      // opzionale (per gestione heredoc aperto)
	struct s_redir *next;
}	t_redir;



Un nodo rappresenta un comando + redir.  
Le pipe sono una lista linkata di nodi.
 
typedef struct s_node 
{
	char      **cmd_argv;  // ex: {"grep", "-i", "foo", NULL}
	t_redir   *red;        // lista di redirection
	struct s_node *next;   // nodo successivo nella pipe
}	t_node;



Lista di variabili d’ambiente in key=value.

typedef struct s_env
{
	char          *key;
	char          *value;
	int           exported;  // 1 se presente in `env`, 0 se solo locale
	struct s_env  *next;
}	t_env;



─────────────────────────────────────────────────────────────
    EXECUTOR STRUCTURES
─────────────────────────────────────────────────────────────


typedef enum e_btin
{
	CMD_ECHO,
	CMD_CD,
	CMD_PWD,
	CMD_EXPORT,
	CMD_UNSET,
	CMD_ENV,
	CMD_EXIT,
	CMD_INVALID
}	t_btin;

#endif

*/