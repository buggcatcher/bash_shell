/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_mini.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/28 21:32:38 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	ft_handle_heredoc(t_redir *redirs)
// {
// 	int		pipefd[2];
// 	pid_t	pid;
// 	int		status;

// 	while (redirs->type == TK_HEREDOC_5)
// 	{
// 		if (pipe(pipefd) == -1)
// 			return (write(2, "Heredoc Pipe\n", 14), 1);
// 		pid = fork();
// 		if (pid == -1)
// 			return (ft_handle_fail_heredoc_child(pipefd), 1);
// 		if (pid == 0)
// 			ft_handle_heredoc_child(pipefd, redirs->filename);
// 		else
// 		{
// 			close(pipefd[1]);
// 			waitpid(pid, &status, 0);
// 			if ((status & 0x7F) == 0 && ((status >> 8) & 0xFF) == 0)
// 				redirs->fd = pipefd[0];
// 			else
// 				return (close(pipefd[0]), 1);
// 		}
// 		redirs = redirs->next;
// 	}
// 	return (0);
// }

// NEW
int	ft_handle_heredoc(t_redir *redirs)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	while (redirs->type == TK_HEREDOC_5)
	{
		if (pipe(pipefd) == -1)
			return (write(2, "Heredoc Pipe\n", 14), 1);
		pid = fork();
		if (pid == -1)
			return (ft_handle_fail_heredoc_child(pipefd), 1);
		if (pid == 0)
			ft_handle_heredoc_child(pipefd, redirs->filename, redirs);
		else
		{
			close(pipefd[1]);
			waitpid(pid, &status, 0);
			if ((status & 0x7F) == 0 && ((status >> 8) & 0xFF) == 0)
				redirs->fd = pipefd[0];
			else
				return (close(pipefd[0]), 1);
		}
		redirs = redirs->next;
	}
	return (0);
}


void	ft_handle_fail_heredoc_child(int pipefd[2])
{
	write(2, "Heredoc Pipe\n", 14);
	close(pipefd[0]);
	close(pipefd[1]);
}

// void	ft_handle_heredoc_child(int pipefd[2], char *delimiter)
// {
// 	close(pipefd[0]);
// 	ft_heredoc_child_process(delimiter, pipefd[1]);
// 	exit(0);
// }

// int	ft_heredoc_child_process(char *delimiter, int write_fd)
// {
// 	char	*line;
// 	size_t	delilen;

// 	delilen = ft_strlen_v(delimiter);
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line)
// 			break ;
// 		if (ft_strncmp(line, delimiter, delilen) == 0 && \
// 			line[delilen] == '\0')
// 		{
// 			free(line);
// 			break ;
// 		}
// 		write(write_fd, line, ft_strlen_v(line));
// 		write(write_fd, "\n", 1);
// 		free(line);
// 	}
// 	close(write_fd);
// 	exit(0);
// }

// NEW
void	ft_handle_heredoc_child(int pipefd[2], char *delimiter, t_redir *redirs)
{
	close(pipefd[0]);
	ft_heredoc_child_process(delimiter, pipefd[1], redirs);
	ft_free_nodes(redirs->node);
	exit(0);
}
//NEW
int	ft_heredoc_child_process(char *delimiter, int write_fd, t_redir *redirs)
{
	char	*line;
	size_t	delilen;

	delilen = ft_strlen_v(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, delilen) == 0 && \
			line[delilen] == '\0')
		{
			free(line);
			break ;
		}
		write(write_fd, line, ft_strlen_v(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	close(write_fd);
	ft_free_nodes(redirs->node);
	exit(0);
}


int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}
