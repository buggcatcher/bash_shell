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

typedef enum e_token_type {
	TK_WORD,     	// qualunque parola non speciale (comando, opzione, argomento)
	TK_PIPE,     	// |
	TK_REDIR_IN, 	// <
	TK_REDIR_OUT,	// >
	TK_REDIR_APPEND,// >>
	TK_HEREDOC,  	// <<
	TK_DOLLAR,   	// $VAR o $?
	TK_QUOTE,    	// " o ' delimitatore (potresti usarlo o gestire quote nel lexer stesso)
	TK_WHITESPACE,  // spazio, tab (usato per splitting o ignorato)
	TK_EOF       	// fine input
} t_token_type;

typedef struct s_token {
	t_token_type type; // tipo del token
	char *value;   	// valore grezzo o parzialmente elaborato
	struct s_token *next;
} t_token;

typedef struct s_redir {
	int type;     	// REDIR_IN, REDIR_OUT, etc.
	char *filename;   // nome file da redirigere
	int fd;       	// 0, 1, 2... o -1 per default
	struct s_redir *next;
} t_redir;

typedef struct s_node {
	char **argv;   	// comando + args: {"ls", "-l", NULL}
	t_redir *redirs;   // lista redirezioni associate
	struct s_node *next; // nodo successivo (pipe)
} t_node;

#endif