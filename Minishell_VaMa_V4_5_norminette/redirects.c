/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/23 19:30:20 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_redirection_file(t_redir *redir)
{
	int	fd;

	if (redir->type == TK_REDIR_IN_2)
		fd = open(redir->filename, O_RDONLY);
	else if (redir->type == TK_REDIR_OUT_3)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == TK_REDIR_APPEND_4)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (-1);
	return (fd);
}

static int	handle_heredoc(t_redir *redir)
{
	if (ft_handle_heredoc(redir) != 0)
		return (1);
	return (0);
}

static int	get_redirection_target(int type)
{
	if (type == TK_REDIR_IN_2 || type == TK_HEREDOC_5)
		return (STDIN_FILENO);
	return (STDOUT_FILENO);
}

static int	process_redirection(t_redir *redir)
{
	int	fd;
	int	target;

	if (redir->type == TK_HEREDOC_5)
	{
		if (handle_heredoc(redir) != 0)
			return (1);
		fd = redir->fd;
	}
	else
	{
		fd = open_redirection_file(redir);
		if (fd < 0)
			return (perror("redirect open error"), 1);
	}
	target = get_redirection_target(redir->type);
	if (switch_fd(fd, target) != 0)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_redirects(t_redir *redirs)
{
	while (redirs)
	{
		if (process_redirection(redirs) != 0)
			return (1);
		redirs = redirs->next;
	}
	return (0);
}
