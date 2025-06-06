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

extern int	exit_status;

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
	char *filename;   // nome file da redirigere
	int fd;       	// 0, 1, 2... o -1 per default
	struct s_redir *next;
} t_redir;

typedef struct s_node 
{
	char **argv;   	// comando + args: {"ls", "-l", NULL}
	t_redir *redirs;   // lista redirezioni associate
	struct s_node *next; // nodo successivo (pipe)
} t_node;

// minishell.c
void	ft_print_token(t_token *token);

// tokenize_mini.c
t_token	*ft_tokenize(t_token *token, char *input);
t_token *ft_get_token(t_token *token, char **input, t_token **new);
t_token	*ft_create_token(t_token_type type, const char *start, int len);
t_token *ft_word(t_token **new, char **input);

// dollar_mini.c
t_token	*ft_dquote(t_token *token, t_token **new, char **input);
void	ft_check_dquote(t_token *token, char *start);
int		ft_check_var(char **input);
char	*ft_create_var(char *buffer, char **input);
char	*ft_expand_var(char **input);

// token_type_mini.c
t_token	*ft_pipe(t_token **new, char **input);
t_token	*ft_redher(t_token **new, char **input);
t_token	*ft_redred(t_token **new, char **input);
t_token	*ft_squote(t_token *token, t_token **new, char **input);

// syntax_check.c
void	ft_check_syntax(t_token *token);
int		ft_is_operator(t_token *token);

// error_mini.c
void	ft_error(t_token *token, char *msg);
void	ft_putstr(char *str);
void	ft_free_token(t_token *token);

// utils_mini.c
size_t	ft_strlen(const char *a);
char	*ft_strndup(const char *s, size_t n);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_append_char(char *s, char c);

// itoa_mini.c
char	*ft_char(char *s, unsigned int number, long int len);
int		ft_nlen(int n);
char	*ft_itoa(int n);


#endif