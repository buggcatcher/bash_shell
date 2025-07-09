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

int	ft_handle_out_append(t_node *node)
{
	t_redir *redir;

	redir = node->redirs;
	while (redir)
	{
		if (redir->type == TK_REDIR_OUT_3) // >
			redir->fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644); // Scrittura(O_WRONLY)- Crea il file se non esiste (O_CREAT) - Sovrascrive il file se esiste (O_TRUNC)
		else if (redir->type == TK_REDIR_APPEND_4) // >>
			redir->fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644); // Scrittura(O_WRONLY) - Non cancella nulla, scrive in fondo (O_APPEND)
		if (redir->fd == -1)
			return (perror("ERROR Open"), -1);
		if (dup2(redir->fd, STDOUT_FILENO) == -1) // duplica fd sul 1 (stdout), ora stdout (fd = 1) punta a file.txt. , tutto ciò che scrive su stdout va in file.txt.
		{
			perror("ERROR Dup2");
			close(redir->fd);
			return (-1);
		}
		close(redir->fd); // chiudi il fd duplicato, dup2 lo ha già copiato su stdout
		redir = redir->next;
	}
	return (0);
}

int	ft_handle_in(t_node *node)
{
	t_redir *redir;

	redir = node->redirs;
	while (redir)
	{
		if (redir->type == TK_REDIR_IN_2) // <
		{
			redir->fd = open(redir->filename, O_RDONLY);
			if (redir->fd == -1)
				return (perror("ERROR open"), -1);

			if (dup2(redir->fd, STDIN_FILENO) == -1)
			{
				perror("ERROR dup2");
				close(redir->fd);
				return (-1);
			}
			close(redir->fd); // dup2 ha già copiato l'fd
		}
		redir = redir->next;
	}
	return (0);
}
