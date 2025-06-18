/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/05/28 12:50:15 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <readline/readline.h>   // readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay
#include <readline/history.h>    // add_history
#include <stdio.h>               // printf, perror
#include <stdlib.h>              // malloc, free, exit, getenv
#include <unistd.h>              // write, access, open, read, close, fork, pipe, dup, dup2, execve, unlink, chdir, getcwd, isatty, ttyname, ttyslot, ioctl
#include <sys/wait.h>            // wait, waitpid, wait3, wait4
#include <signal.h>              // signal, sigaction, sigemptyset, sigaddset, kill
#include <sys/stat.h>            // stat, lstat, fstat
#include <dirent.h>              // opendir, readdir, closedir
#include <string.h>              // strerror
#include <termios.h>             // tcsetattr, tcgetattr
#include <stdbool.h>			// boolean variable
#include <fcntl.h>				// open, O_RDONLY, O_WRONLY, O_CREAT, ecc.
#include <errno.h>				// errno
#include <ctype.h>

#define PATH_MAX 4096

extern bool	exit_status; // da rimuovere. gestita modularmente con struct t_shell_state nel suo contesto

extern volatile sig_atomic_t g_sigint;

typedef enum e_token_type 
{
	TK_WORD_0,     	// qualunque parola non speciale (comando, opzione, argomento)
	TK_PIPE_1,     	// |
	TK_REDIR_IN_2, 	// <
	TK_REDIR_OUT_3,	// >
	TK_REDIR_APPEND_4, // >>
	TK_HEREDOC_5,  	// <<
	TK_S_QUOTE_6, // ' delimitatore (potresti usarlo o gestire quote nel lexer stesso)
	TK_D_QUOTE_7,  // " delimitatore (potresti usarlo o gestire quote nel lexer stesso)
	TK_DOLLAR_8, // $ o $VAR
} t_token_type;

typedef struct s_token 
{
	t_token_type type; // tipo del token
	char *value;   	// valore grezzo o parzialmente elaborato
	struct s_token *next;
} t_token;

typedef struct s_redir 
{
	int type;     	// REDIR_IN, REDIR_OUT, etc.
	char *filename;   // nome filename da redirigere
	int fd;       	// 0, 1, 2... o -1 per default //cambiato in un singolo intero, perché per l'heredoc ora serve solo il lato di lettura della pipe.
	struct s_redir *next;
} t_redir;

typedef struct s_node 
{
	char **argv;   	// comando + args: {"ls", "-l", NULL}
	t_redir *redirs;   // lista redirezioni associate
	struct s_node *next; // nodo successivo (pipe)
} t_node;

// Variabili di ambiente come lista linkata
typedef struct s_env {
    char *key;
    char *value;
    int exported; // 1 = passa a execve() e mostra in env. 0 = visibile nella shell ($VAR), ma non nelle child
    struct s_env *next;
} t_env;

typedef struct s_shell_state
{
    t_env *env;
    int last_status;        // Exit code dell'ultimo comando (per $?)
    int should_exit;        // Flag per uscire dal main loop
    int exit_code;          // Codice di uscita finale per exit()
} t_shell_state;

// minishell.c
void	ft_print_token(t_token *token);
void	ft_print_nodes(t_node *node);

// tokenize_mini.c
t_token	*ft_tokenize(t_shell_state *state, t_token *token, char *input);
t_token *ft_get_token(t_shell_state *state, t_token *token, char **input, t_token **new);
t_token	*ft_create_token(t_token_type type, char *start, int len);
t_token *ft_word(t_shell_state *state, t_token **new, char **input);

// dollar_mini.c
t_token	*ft_dquote(t_shell_state *state, t_token *token, t_token **new, char **input);
void	ft_check_dquote(t_shell_state *state, t_token *token, char *start);
int		ft_check_var(char **input);
char	*ft_create_var(char *buffer, char **input, t_shell_state *state);
char	*ft_expand_var(char **input, t_shell_state *state);

// token_type_mini.c
t_token	*ft_pipe(t_token **new, char **input);
t_token	*ft_redher(t_token **new, char **input);
t_token	*ft_redred(t_token **new, char **input);
t_token	*ft_squote(t_token *token, t_token **new, char **input);

// syntax_check.c
int		ft_check_syntax(t_token *token);
int		ft_is_operator(t_token *token);

// node.mini.c
t_node	*ft_node(t_token *token);
t_node	*ft_get_node(t_token *token);
void	ft_create_node(t_node **head, t_node *new);
void	ft_advance_tokens(t_token **start, t_token **tmp);
void	ft_add_redirection(t_node *node, t_token *token);

// node_argv_mini.c
char	**ft_build_argv(t_token *start, t_token *end);
int		ft_count_tokens(t_token *start, t_token *end);
char	**populate_argv(char **argv, t_token *start, t_token *end);
t_token	*skip_redirection_tokens(t_token *tmp);
int		ft_argv(t_token *tmp, char **argv, int i);

// redirections_mini.c
int		ft_handle_out_append(t_node *node);
int		ft_handle_in(t_node *node);

// heredoc_mini.c
int		ft_handle_heredoc(t_redir *redirs);
void	ft_handle_fail_heredoc_child(int pipefd[2]);
void	ft_handle_heredoc_child(int pipefd[2], char *delimiter);
int		ft_heredoc_child_process(char *delimiter, int write_fd);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

// free_mini.c
void	ft_free_token(t_token *token);
t_node	*ft_free_nodes(t_node *head);
void	ft_free_argv(char **argv);
void	ft_free_redirs(t_redir *redir);

// error_mini.c
int		ft_error(t_token *token, char *msg);
void	ft_putstr(char *str);

// utils_mini.c
size_t	ft_strlen_v(char *a);
char	*ft_strndup(char *s, size_t n);
char	*ft_strdup(char *s);
char	*ft_strjoin(char *s1, char *s2);
char	*ft_append_char(char *s, char c);

// itoa_mini.c
char	*ft_char(char *s, unsigned int number, long int len);
int		ft_nlen(int n);
char	*ft_itoa(int n);


// === exe.c === //
int		exe_pipeline(t_node *head, t_env **env);
int		exe_single_cmd(t_node *node, t_env **env);
int		exec_bin(char **argv, t_env **env);
int		exec_builtin(char **argv, t_env **env);
char	*join_path(char *dir, char *cmd);
char	*get_env_value(char *key, t_env *env);
void	free_split_all(char **arr);
int		apply_redirects(t_redir *redirs);
char	**env_to_array(t_env *env);
char	*resolve_path(char *cmd, t_env *env);
t_env	*init_env_from_system(char **envp);
void	init_shell_state(t_shell_state *state, char **envp);
void	cleanup_shell_state(t_shell_state *state);
int		executor_loop(t_node *node, t_shell_state *state);




// === btin.c === //
void	set_env_var(t_env **env,  char *key,  char *value);
int		exe_cd(char **args, t_env **env);
int		exe_pwd(char **args, t_env **env);
int		exe_echo(char **args);
int		is_parent_builtin(char *cmd);
int		is_child_builtin(char *cmd);
int		should_execute_in_parent(t_node *node);
int		is_builtin(char *cmd);
t_env	*find_env_node(t_env *env,  char *key);
t_env	*create_env_node( char *key,  char *value, int exported);
int		remove_env_node(t_env **env,  char *key);
int		exe_export(char **args, t_env **env);
int		exe_unset(char **args, t_env **env);
int		exe_env(t_env *env);
int		exe_exit(char **args, t_shell_state *state);


/// == utils.c == ///
// --- fd 
int		switch_fd(int from, int to);
int		save_fd(int fd);
int		save_stdout(void);
int		open_outfile( char *filename);
// --- memory 
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t n_elem, size_t size);
void	*safe_alloc(size_t n_elem, size_t size,  char *desc);
// --- libfx 
char	*ft_strchr( char *s, int c);
char	*ft_strjoin_m( char *s1,  char *s2);
char	*ft_strdup_m( char *s);
char	**ft_split( char *s, char c);
size_t	ft_strlen( char *s);
void	free_array(char **arr);
int		env_size(t_env *env);
int		ft_strcmp(char *s1, char *s2);
int		is_numeric(const char *str);
long	ft_strtol(const char *str, char **endptr);
void	ft_putstr_stderr(char *s);
void	handle_sigint(int sig);
void	setup_signals(void);



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