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

#include "minishell.h"

// int	handle_heredoc(t_redir *redirs)
// {
// 	int		pipefd[2];
// 	pid_t	pid;
// 	int		status;

// 	if (pipe(pipefd) == -1)
// 	{
// 		perror("Heredoc Pipe");
// 		return (1);
// 	}
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("Heredoc Fork");
// 		close(pipefd[0]);
// 		close(pipefd[1]);
// 		return (1);
// 	}
// 	if (pid == 0)
// 	{
// 		close(pipefd[0]);  // Chiudi lato lettura nel figlio
// 		heredoc_child_process(redirs->filename, pipefd[1]);
// 		exit(0); // Assicurati che il figlio esca esplicitamente
// 	}
// 	else
// 	{
// 		close(pipefd[1]);  // Chiudi lato scrittura nel padre
// 		waitpid(pid, &status, 0);
// 		if ((status & 0x7F) == 0 && ((status >> 8) & 0xFF) == 0) // Verifica che il figlio sia uscito normalmente con codice 0
// 			redirs->fd[0] = pipefd[0]; // Valido solo se heredoc è OK
// 		else
// 		{
// 			close(pipefd[0]); // Fallito: chiudi e lascia -1
// 			return (1);
// 		}
// 	}
// 	return (0);
// }

int	ft_handle_heredoc(t_redir *redirs)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;

	if (pipe(pipefd) == -1)
		return (perror("Heredoc Pipe"), 1);
	pid = fork();
	if (pid == -1)
		return (ft_handle_fail_heredoc_child(&pipefd[2]), 1);
	if (pid == 0)
		ft_handle_heredoc_child(pipefd, redirs->filename);
	else
	{
		close(pipefd[1]);  // Chiudi lato scrittura nel padre
		waitpid(pid, &status, 0);
		if ((status & 0x7F) == 0 && ((status >> 8) & 0xFF) == 0) // Verifica che il figlio sia uscito normalmente con codice 0
			redirs->fd[0] = pipefd[0]; // Valido solo se heredoc è OK
		else
			return (close(pipefd[0]), 1); // Fallito: chiudi e lascia -1
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
	close(pipefd[0]); // Chiudi lato lettura nel figlio
	ft_heredoc_child_process(delimiter, pipefd[1]);
	exit(0); // Termina il processo figlio
}

// 	// Ignora segnali nel processo figlio (opzionale ma consigliato)
// 	//signal(SIGINT, SIG_IGN); DA RIVEDERE IN UN SECONDO MOMENTO

int	ft_heredoc_child_process(char *delimiter, int write_fd)
{
	char *line;
	size_t delilen;

	delilen = ft_strlen_v(delimiter);
	while (1)
	{
		line = readline("> ");  // prompt
		if (!line)  // Ctrl+D o errore
			break;
		if (ft_strncmp(line, delimiter, delilen) == 0 && // Controlla se è il delimiter
			line[delilen] == '\0')
		{
			free(line);
			break;
		}
		write(write_fd, line, ft_strlen_v(line)); // Scrive su write_fd con newline finale
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
