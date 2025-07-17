/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_mini.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/07/17 14:39:28 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*ft_node(t_token *token)
{
	t_token	*start;
	t_token	*tmp;
	t_node	*new;
	t_node	*head;

	head = NULL;
	start = token;
	tmp = token;
	while (tmp)
	{
		while (tmp && tmp->type != TK_PIPE_1)
			tmp = tmp->next;
		new = ft_get_node(start);
		if (!new)
			return (ft_free_nodes(head));
		if ((new->token->type == TK_S_QUOTE_6 || new->token->type == TK_D_QUOTE_7 \
				|| new->token->type == TK_DOLLAR_8) || new->token->type == TK_WORD_0)
			new->argv = ft_build_argv(start, tmp);
		if (!new->argv && new->redirs == NULL)
			return (ft_free_nodes(new));
		ft_create_node(&head, new);
		ft_advance_tokens(&start, &tmp);
	}
	return (head);
}

static t_token	*ft_find_first_cmd_token(t_token *token)
{
	t_token	*tmp = token;
	t_token	*first_cmd_token;

	first_cmd_token = NULL;
	while (tmp && tmp->type != TK_PIPE_1)
	{
		if (tmp->type >= TK_REDIR_IN_2 && tmp->type <= TK_HEREDOC_5)
		{
			if (tmp->next)
				tmp = tmp->next;
		}
		else if (tmp->type == TK_WORD_0 || tmp->type == TK_S_QUOTE_6 || \
				tmp->type == TK_D_QUOTE_7 || tmp->type == TK_DOLLAR_8)
		{
			first_cmd_token = tmp;
			break;
		}
		tmp = tmp->next;
	}
	return (first_cmd_token);
}

static void	ft_process_redirections(t_node *node)
{
	t_token *tmp;
	
	tmp = node->token;
	while (tmp && tmp->type != TK_PIPE_1)
	{
		if (tmp->type >= TK_REDIR_IN_2 && tmp->type <= TK_HEREDOC_5)
		{
			ft_add_redirection(node, tmp);
			if (tmp->next)
				tmp = tmp->next;
		}
		tmp = tmp->next;
	}
}

t_node	*ft_get_node(t_token *token)
{
	t_node *new;
	t_token *first_cmd_token;
	
	new = malloc(sizeof(t_node));
	if (!new)
		return (NULL);

	new->redirs = NULL;
	new->token = token;
	new->argv = NULL;
	new->next = NULL;
	first_cmd_token = ft_find_first_cmd_token(token);
	ft_process_redirections(new);
	if (first_cmd_token)
		new->token = first_cmd_token;
	return (new);
}

void	ft_create_node(t_node **head, t_node *new)
{
	t_node	*tail;

	if (!*head)
	{
		*head = new;
	}
	else
	{
		tail = *head;
		while (tail->next)
			tail = tail->next;
		tail->next = new;
	}
}

void	ft_advance_tokens(t_token **start, t_token **tmp)
{
	if (*tmp && (*tmp)->type == TK_PIPE_1)
	{
		*start = (*tmp)->next;
		*tmp = (*tmp)->next;
	}
}

static void	init_redir(t_redir *redir, int type, t_node *node)
{
	redir->node = node;
	redir->type = type;
	redir->fd = -1;
	redir->next = NULL;
	redir->last = NULL;
	redir->heredoc_buffer = NULL;
}

static char	*ft_strdup_trim_last_space(char *s)
{
	size_t	len;
	char	*dup;

	if (!s)
		return (NULL);
	len = ft_strlen_v(s);
	if (len == 0)
		return (ft_strdup(""));
	if (s[len - 1] == ' ')
	{
		dup = (char *)malloc(len);
		if (!dup)
			return (NULL);
		ft_memcpy(dup, s, len - 1);
		dup[len - 1] = '\0';
		return (dup);
	}
	return (ft_strdup(s));
}

void	ft_add_redirection(t_node *node, t_token *token)
{
	t_redir	*new_redir;
	t_redir	*last;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return ;
	init_redir(new_redir, token->type, node);
	if (token->next && (token->next->type == TK_WORD_0 || \
			token->next->type == TK_S_QUOTE_6 || \
			token->next->type == TK_D_QUOTE_7 || \
			token->next->type == TK_DOLLAR_8))
		new_redir->filename = ft_strdup_trim_last_space(token->next->value);
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
