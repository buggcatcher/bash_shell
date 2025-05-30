/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:29:24 by vloddo            #+#    #+#             */
/*   Updated: 2025/02/28 13:29:27 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stddef.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <ctype.h>
# include <limits.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>

typedef struct s_pipex
{
	int		infd;
	int		outfd;
	int		pipe_fd[2];
	int		p_id;
	int		p_id_2;
	char	*cmd;
	char	**arg_cmd;
	char	*cmd2;
	char	**arg_cmd2;
	char	**paths;
}			t_pipex;

int			ft_strncmp(const char *s1, const char *s2, size_t n);
size_t		ft_strlen(const char *a);
char		*ft_strjoin(char const *s1, char const *s2);
char		**ft_get_path(char **envp);
char		*check_args(char **paths, char *argcmd);

void		close_data(t_pipex *data);
void		free_memory(t_pipex *data, int i);
void		ft_free_split(char **split_array);
void		ft_error(char *str, int error);
void		error_check(t_pipex *pipex, int free_level, \
			char *msg, int exit_code);

char		**ft_split(const char *s, char c);

#endif