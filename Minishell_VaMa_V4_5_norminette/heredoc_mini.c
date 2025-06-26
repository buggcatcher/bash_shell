/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_mini.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/26 17:22:02 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_handle_heredoc(t_redir *redirs)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	// while (redirs->type != TK_HEREDOC_5)
	// 	redirs = redirs->next;
	if (pipe(pipefd) == -1)
		return (perror("Heredoc Pipe"), 1);
	pid = fork();
	if (pid == -1)
		return (ft_handle_fail_heredoc_child(pipefd), 1);
	if (pid == 0)
		ft_handle_heredoc_child(pipefd, redirs->filename);
	else
	{
		close(pipefd[1]);
		waitpid(pid, &status, 0);
		if ((status & 0x7F) == 0 && ((status >> 8) & 0xFF) == 0)
			redirs->fd = pipefd[0];
		else
			return (close(pipefd[0]), 1);
	}
	return (0);
}

int ft_handle_heredoc(t_redir *redirs)
{
    int last_fd = -1;
    t_redir *current = redirs;
    pid_t pid;
    int status;

    // Processa tutti gli heredoc nella lista
    while (current != NULL)
    {
        if (current->type == TK_HEREDOC_5)
        {
            int pipefd[2];
            
            // Chiudi il precedente heredoc se esiste
            if (last_fd != -1)
                close(last_fd);
            
            if (pipe(pipefd) == -1)
                return (perror("Heredoc Pipe"), 1);
            
            pid = fork();
            if (pid == -1)
                return (close(pipefd[0]), close(pipefd[1]), perror("Heredoc Fork"), 1);
            
            if (pid == 0)
            {
                close(pipefd[0]);
                ft_handle_heredoc_child(pipefd[1], current->filename);
                close(pipefd[1]);
                exit(EXIT_SUCCESS);
            }
            else
            {
                close(pipefd[1]);
                waitpid(pid, &status, 0);
                
                if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
                {
                    last_fd = pipefd[0];  // Salva il fd per potenziale chiusura
                }
                else
                {
                    close(pipefd[0]);
                    return (1);
                }
            }
        }
        current = current->next;
    }

    // Assegna l'ultimo fd trovato all'ultimo heredoc
    if (last_fd != -1)
    {
        while (redirs != NULL)
        {
            if (redirs->type == TK_HEREDOC_5)
            {
                // Chiudi eventuali fd precedenti assegnati
                if (redirs->fd != -1)
                    close(redirs->fd);
                    
                redirs->fd = last_fd;
            }
            redirs = redirs->next;
        }
    }
    
    return (0);
}

void	ft_handle_fail_heredoc_child(int pipefd[2])
{
	perror("Heredoc Fork");
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
