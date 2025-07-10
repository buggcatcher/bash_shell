/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/07/10 15:27:25 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <termios.h>
# include <stdbool.h>
# include <fcntl.h>
# include <errno.h>
# include <ctype.h>
# define PATH_MAX 4096

extern volatile sig_atomic_t	g_sigint;

typedef enum e_token_type
{
	TK_WORD_0,
	TK_PIPE_1,
	TK_REDIR_IN_2,
	TK_REDIR_OUT_3,
	TK_REDIR_APPEND_4,
	TK_HEREDOC_5,
	TK_S_QUOTE_6,
	TK_D_QUOTE_7,
	TK_DOLLAR_8,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	int				wspace;
	struct s_token	*next;
}	t_token;

typedef struct s_node t_node;
typedef struct s_heredoc_buffer t_heredoc_buffer;

typedef struct s_redir
{
    int type;
    char *filename;
    int fd;
    t_node *node;
    t_heredoc_buffer *heredoc_buffer;  // new
    struct s_redir *next;
    struct s_redir *last;
} t_redir;

typedef struct s_node
{
	t_redir			*redirs;
	t_token			*token;
	char			**argv;
	struct s_node	*next;
}	t_node;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				exported;
	struct s_env	*next;
}	t_env;

typedef struct s_shell_state
{
	t_env	*env;
	int		last_status;
	int		should_exit;
	int		exit_code;
}	t_shell_state;

typedef struct s_heredoc_buffer
{
    char *content;
    size_t size;
    size_t capacity;
} t_heredoc_buffer;

// main.c
char	*read_user_input(t_shell_state *state);
bool	process_input(char *input, t_shell_state *state);
void	shell_loop(t_shell_state *state);

// tokenize_mini.c
t_token	*ft_tokenize(t_shell_state *state, t_token *token, char *input);
t_token	*ft_get_token(t_shell_state *s, t_token *t, char **i, t_token **n);
t_token	*ft_create_token(t_token_type type, char *start, int len);
t_token	*ft_word(t_shell_state *state, t_token **new, char **input);

// dollar_mini.c
int		ft_check_dquote(t_shell_state *state, t_token *token, char *start);
int		ft_check_var(char **input);
char	*ft_create_var(char *buffer, char **input, t_shell_state *state);
char	*ft_expand_var(char **input, t_shell_state *state);
char	*ft_my_getenv(char *var_name, t_shell_state *state);

// token_type_mini.c
t_token	*ft_pipe(t_token **new, char **input);
t_token	*ft_redher(t_token **new, char **input);
t_token	*ft_redred(t_token **new, char **input);
t_token	*ft_squote(t_token *token, t_token **new, char **input);
t_token	*ft_dquote(t_shell_state *s, t_token *t, t_token **n, char **i);

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
//t_token	*skip_redirection_tokens(t_token *tmp);
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
void	ft_free_env(t_env *env);
t_node	*ft_free_nodes(t_node *head);
void	ft_free_token(t_token *token);
void	ft_free_argv(char **argv);
void	ft_free_redirs(t_redir *redir);
void	free_array(char **arr);
void	clean_exit(t_node *node, t_env *env, int status);

// error_mini.c
//int		ft_error(t_token *token, char *msg);
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

// === btin_apply.c === //
char	*join_key_value(char *key, char *value);
int		spawn_forked_command(t_node *n, int pipe_fd[2], int prev_fd, t_env *e);
int		exec_builtin(char **args, t_env **env);
int		handle_builtin_if_applicable(t_node *n, t_env **e, int *last_status);
int		create_pipe_if_needed(t_node *node, int pipe_fd[2]);

// === btin_search.c === //
int		is_parent_builtin(char *cmd);
int		is_child_builtin(char *cmd);
int		should_execute_in_parent(t_node *node);
int		is_builtin(char *cmd);

// === executor_loop.c === //
int		exec_in_parent(t_node *node, t_shell_state *state);
void	update_fds_for_next_cmd(t_node *node, int pipe_fd[2], int *prev_fd);
int		wait_for_last(pid_t last_pid, int *exit_status);
int		executor_loop(t_node *node, t_shell_state *state);

// === redirect.c === //
int		apply_redirects(t_redir *redirs);

// === resolve_path.c === //
//char	*resolve_path(char *cmd, t_env *env);
char	*resolve_path(char *cmd, t_env *env, t_node *node);

// === child.c === //
void	exec_child(t_node *node, int pipe_out[2], int pipe_in, t_env *env);

// === various.c === //
void	free_split_all(char **arr);
char	*join_path(char *dir, char *cmd);
char	*get_env_value(char *key, t_env *env);
void	init_shell_state(t_shell_state *state, char **envp);
void	cleanup_shell_state(t_shell_state *state);

// === system_env.c === //
void	cleanup_node(t_env *node);
t_env	*create_env_node_m(char *env_entry);
t_env	*process_env_entry(char *env_entry, t_env *env);
t_env	*init_env_from_system(char **envp);
char	**env_to_array(t_env *env);

// === env.c === //
void	set_env_var(t_env **env, char *key, char *value);
t_env	*find_env_node(t_env *env, char *key);
t_env	*create_env_node( char *key, char *value, int exported);
int		remove_env_node(t_env **env, char *key);

// === exe_btin.c === //
int		exe_pwd(char **args, t_env **env);
int		exe_unset(char **args, t_env **env);
int		exe_env(t_env *env);
int		exe_echo(char **args);

// === exe_cd.c === //
int		get_cwd_or_error(char *buf, const char *context);
char	*resolve_cd_target(char **args, t_env *env);
int		exe_cd(char **args, t_env **env);

// === exe_exit.c === //
int		exit_numeric_error(char *arg, t_shell_state *state);
int		exit_too_many_args(t_shell_state *state);
int		exe_exit(char **args, t_shell_state *state);

// === exe_export_parse.c === //
bool	is_valid_variable_name(const char *str);
void	print_export_error(char *arg);

// === exe_export.c === //
void	handle_export_without_value(char *arg, t_env **env);
void	handle_key_value_export(char *arg, char *equals, t_env **env);
int		handle_single_export(char *arg, t_env **env);
int		print_exported_env(t_env *env);
int		exe_export(char **args, t_env **env);

/// === utils.c === ///
char	*ft_strchr(char *s, int c);
char	*ft_strjoin_m(char *s1, char *s2);
char	*ft_strdup_m(char *s);
size_t	ft_strlen(char *s);
int		ft_strcmp(char *s1, char *s2);

/// === split.c === ///
char	**ft_split(char *s, char c);

/// === libfx.c === ///
int		env_size(t_env *env);
int		is_numeric(const char *str);
long	ft_strtol(const char *str, char **endptr);
void	ft_putstr_stderr(char *s);
void	*ft_memcpy(void *dst, const void *src, size_t n);
char	*ft_strcpy(char *dst, const char *src);

/// === fd.c === ///
int	switch_fd(int src, int dest);
int		save_fd(int fd);
int		save_stdout(void);
int		open_outfile( char *filename);

/// === memory.c === ///
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t n_elem, size_t size);
void	*safe_alloc(size_t n_elem, size_t size, char *desc);

/// === signals.c === ///
void	handle_sigint(int sig);
void	setup_signals(void);
void	disable_signals(void);
void	ignore_signals(void);
void	free_env_list(t_env *env);


///heredoc
int handle_heredoc_sequence(t_redir *start_redir);
int read_heredoc_input(char *delimiter, t_heredoc_buffer *buffer);
int preprocess_heredocs(t_node *nodes);
int append_to_buffer(t_heredoc_buffer *buffer, const char *line);
void free_heredoc_buffer(t_heredoc_buffer *buffer);
int handle_heredoc_redirection(t_redir *redir);
int create_fd_from_buffer(t_heredoc_buffer *buffer);
int fill_heredoc_buffer(t_redir *start, t_heredoc_buffer *buffer);
void mark_as_skipped(t_redir *start);



void debug_print_nodes(t_node *node);

#endif