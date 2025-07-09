/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_mini.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/30 18:50:25 by vloddo           ###   ########.fr       */
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
		new->argv = ft_build_argv(start, tmp);
		if (!new->argv)
		{
			free(new);
			return (ft_free_nodes(head));
		}
		ft_create_node(&head, new);
		ft_advance_tokens(&start, &tmp);
	}
	return (head);
}

t_node	*ft_get_node(t_token *token)
{
	t_node	*new;
	t_token	*tmp;

	new = malloc(sizeof(t_node));
	if (!new)
		return (NULL);
	new->redirs = NULL;
	new->token = token;
	new->argv = NULL;
	new->next = NULL;
	tmp = token;
	while (tmp && tmp->type != TK_PIPE_1)
	{
		if (tmp->type >= TK_REDIR_IN_2 && tmp->type <= TK_HEREDOC_5)
		{
			ft_add_redirection(new, tmp);
			if (tmp->next)
				tmp = tmp->next;
		}
		tmp = tmp->next;
	}
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
