/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_redirection_mini.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/30 20:07:01 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	init_redir(t_redir *redir, int type)
{
	redir->type = type;
	redir->fd = -1;
	redir->next = NULL;
}

void	ft_add_redirection(t_node *node, t_token *token)
{
	t_redir	*new_redir;
	t_redir	*last;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return ;
	init_redir(new_redir, token->type);
	if (token->next && (token->next->type == TK_WORD_0 || \
			token->next->type == TK_S_QUOTE_6 || \
			token->next->type == TK_D_QUOTE_7 || \
			token->next->type == TK_DOLLAR_8))
		new_redir->filename = ft_strdup(token->next->value);
	else
		new_redir->filename = NULL;
	if (!node->redirs)
		node->redirs = new_redir;
	else
	{
		last = node->redirs;
		while (last->next)
			last = last->next;
		last->next = new_redir;
		new_redir->last = last;
	}
}
