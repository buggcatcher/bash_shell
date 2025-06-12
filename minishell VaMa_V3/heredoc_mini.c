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

// 		// Verifica che il figlio sia uscito normalmente con codice 0
// 		if ((status & 0x7F) == 0 && ((status >> 8) & 0xFF) == 0)
// 		{
// 			redirs->herfd[0] = pipefd[0]; // Valido solo se heredoc è OK
// 		}
// 		else
// 		{mp
// 			close(pipefd[0]); // Fallito: chiudi e lascia -1
// 			return (1);
// 		}
// 	}
// 	return (0);
// }

// 	// Ignora segnali nel processo figlio (opzionale ma consigliato)
// 	//signal(SIGINT, SIG_IGN); DA RIVEDERE IN UN SECONDO MOMENTO

// int	heredoc_child_process(char *delimiter, int write_fd)
// {
// 	char *line;
// 	size_t delilen;

// 	delilen = ft_strlen_v(delimiter);
// 	while (1)
// 	{
// 		line = readline("> ");  // prompt
// 		if (!line)  // Ctrl+D o errore
// 			break;
// 		if (strncmp(line, delimiter, delilen) == 0 && // Controlla se è il delimiter
// 			line[delilen] == '\0')
// 		{
// 			free(line);
// 			break;
// 		}
// 		write(write_fd, line, ft_strlen_v(line)); // Scrive su write_fd con newline finale
// 		write(write_fd, "\n", 1);
// 		free(line);
// 	}
// 	close(write_fd);
// 	exit(0);
// }


#include <signal.h> // Add for signal handling

int handle_heredoc(t_redir *redirs)
{
	int pipefd[2];
	pid_t pid;
	int status;

	// Initialize herfd to -1 in case of early failure
	redirs->herfd[0] = -1;

	if (pipe(pipefd) == -1)
		return (perror("Heredoc Pipe"), 1);

	pid = fork();
	if (pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (perror("Heredoc Fork"), 1);
	}

	if (pid == 0)
	{
		// Child process - set up signal handling
		signal(SIGINT, SIG_DFL); // Handle Ctrl-C properly
		signal(SIGQUIT, SIG_IGN);
		
		close(pipefd[0]);
		heredoc_child_process(redirs->filename, pipefd[1]);
		exit(0); // Explicit exit
	}
	else
	{
		close(pipefd[1]); // Close write end in parent
		waitpid(pid, &status, 0);

		if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		{
			redirs->herfd[0] = pipefd[0]; // Success
		}
		else
		{
			close(pipefd[0]); // Close read end
			redirs->herfd[0] = -1; // CRITICAL: Mark as invalid
			return (1); // Return error
		}
	}
	return (0);
}

int heredoc_child_process(char *delimiter, int write_fd)
{
	char *line;
	size_t delilen = ft_strlen_v(delimiter);

	while (1)
	{
		line = readline("> ");
		if (!line) // Handle EOF (Ctrl+D)
		{
			perror("document delimited by end-of-file\n");
			break;
		}
		
		// Check for exact delimiter match
		if (ft_strlen_v(line) == delilen && 
			!strncmp(line, delimiter, delilen))
		{
			free(line);
			break;
		}
		
		write(write_fd, line, ft_strlen_v(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	close(write_fd);
	exit(0);
}