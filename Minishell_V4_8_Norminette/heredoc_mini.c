/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_mini.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/30 20:11:06 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_loop_heredoc(t_redir *redirs)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipefd) == -1)
		return (write(2, "Heredoc Pipe\n", 14), 1);
	pid = fork();
	if (pid == -1)
		return (close(pipefd[0]), close(pipefd[1]), 1);
	if (pid == 0)
	{
		ft_handle_heredoc_child(pipefd, redirs->filename);
		exit(0);
	}
	close(pipefd[1]);
	waitpid(pid, &status, 0);
	if ((status & 0x7F) == 0 && ((status >> 8) & 0xFF) == 0)
	{
		redirs->fd = pipefd[0];
		return (0);
	}
	close(pipefd[0]);
	return (1);
}

void	ft_handle_fail_heredoc_child(int pipefd[2])
{
	write(2, "Heredoc Pipe\n", 14);
	close(pipefd[0]);
	close(pipefd[1]);
}

void	ft_handle_heredoc_child(int pipefd[2], char *delimiter)
{
	close(pipefd[0]);
	ft_heredoc_child_process(delimiter, pipefd[1]);
	exit(0);
}

int	ft_heredoc_child_process(char *delimiter, int write_fd)
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
	exit(0);
}

int	ft_handle_heredoc(t_redir *redirs)
{
	while (redirs != NULL && redirs->type == TK_HEREDOC_5)
	{
		if (ft_loop_heredoc(redirs) != 0)
			return (1);
		if (redirs->next && redirs->next->fd > 2)
			close(redirs->next->fd);
		redirs = redirs->next;
	}
	return (0);
}
