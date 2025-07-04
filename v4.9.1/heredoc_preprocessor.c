/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_mini.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/29 17:05:57 by vloddo           ###   ########.fr       */
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
// int	ft_handle_heredoc(t_redir *redirs)
// {
// 	int		pipefd[2];
// 	pid_t	pid;
// 	int		status;

// 	while (redirs != NULL && redirs->type == TK_HEREDOC_5)
// 	{
// 		if (pipe(pipefd) == -1)
// 			return (write(2, "Heredoc Pipe\n", 14), 1);
// 		pid = fork();
// 		if (pid == -1)
// 			return (ft_handle_fail_heredoc_child(pipefd), 1);
// 		if (pid == 0)
// 			ft_handle_heredoc_child(pipefd, redirs->filename, redirs);
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

//NEW 2
// int	ft_handle_heredoc(t_redir *redirs)
// {
// 	int		pipefd[2];
// 	pid_t	pid;
// 	int		status;

// 	while (redirs != NULL && redirs->type == TK_HEREDOC_5)
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
// 		if (redirs->next && redirs->next->fd > 2)
// 			close(redirs->next->fd);
// 		redirs = redirs->next;
// 	}
// 	return (0);
// }

// void	ft_handle_fail_heredoc_child(int pipefd[2])
// {
// 	write(2, "Heredoc Pipe\n", 14);
// 	close(pipefd[0]);
// 	close(pipefd[1]);
// }

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
// 		if (ft_strncmp(line, delimiter, delilen) == 0 && 
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
// void	ft_handle_heredoc_child(int pipefd[2], char *delimiter)
// {
// 	close(pipefd[0]);
// 	ft_heredoc_child_process(delimiter, pipefd[1]);
// 	//addition`
// 	// free(delimiter);
// 	// close(pipefd[1]);
// 	// exit(0);
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
// 		if (ft_strncmp(line, delimiter, delilen) == 0 && 
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
// 	free(delimiter);
// 	exit(0);
// }





/*
Legge riga per riga dall’input standard finché non incontra il delimitatore specificato. 
Per ogni riga letta, la passa a append_to_buffer. 
Se l’utente inserisce il delimitatore, la lettura si interrompe.
*/
int read_heredoc_input(char *delimiter, t_heredoc_buffer *buffer)
{
    char *line;
    size_t delimiter_len;
    
    delimiter_len = ft_strlen(delimiter);
    while (1)
    {
        line = readline("> ");
        if (!line)
            break;
        if (ft_strlen(line) == delimiter_len && 
            strncmp(line, delimiter, delimiter_len) == 0)
        {
            free(line);
            break;
        }
        if (append_to_buffer(buffer, line) != 0)
        {
            free(line);
            return (1);
        }
        free(line);
    }
    return (0);
}

/*
Si occupa di scorrere tutte le redirezioni di un singolo nodo, e per ogni 
redirezione di tipo heredoc che non è ancora stata processata (fd == -1), 
chiama handle_heredoc_sequence per gestirla. 
Se una qualsiasi di queste chiamate fallisce, interrompe l’esecuzione e segnala l’errore.
*/
static int process_node_heredocs(t_node *node)
{
    t_redir *redir = node->redirs;
    
    while (redir)
    {
        if (redir->type == TK_HEREDOC_5 && redir->fd == -1)
        {
            if (handle_heredoc_sequence(redir) != 0)
                return (1);
        }
        redir = redir->next;
    }
    return (0);
}

/*
Cicla su una lista di nodi, e per ciascuno di essi chiama process_node_heredocs, 
quindi gestisce gli heredoc per tutta la catena di nodi. 
Se il processo fallisce per un nodo, ritorna l’errore immediatamente.
*/
int preprocess_heredocs(t_node *nodes)
{
    t_node *current = nodes;
    
    while (current)
    {
        if (process_node_heredocs(current) != 0)
            return (1);
        current = current->next;
    }
    return (0);
}

/*
La funzione fill_heredoc_buffer legge sequenzialmente tutti gli heredoc consecutivi 
a partire da una redirezione, leggendo l’input fino al delimitatore e accumulandolo nel buffer.
Se la lettura fallisce in qualsiasi punto, ritorna un errore.
*/
int fill_heredoc_buffer(t_redir *start, t_heredoc_buffer *buffer)
{
    t_redir *current = start;

    while (current && current->type == TK_HEREDOC_5)
    {
        if (read_heredoc_input(current->filename, buffer) != 0)
            return (1);
        current = current->next;
    }
    return (0);
}

/*
Scorre tutte le redirezioni heredoc successive e le marca con fd = -2 per indicare 
che sono state già gestite e devono essere saltate nel processamento successivo.
*/
void mark_as_skipped(t_redir *start)
{
    t_redir *current = start;

    while (current && current->type == TK_HEREDOC_5)
    {
        current->fd = -2;
        current = current->next;
    }
}







