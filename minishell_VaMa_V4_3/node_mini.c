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

/*
t_node	*ft_node(t_token *token, t_node *node)
{
	t_node *head;
	t_node *tail;
	t_token *start;
	t_token *tmp;
	t_node *new;

	head = NULL;
	tail = NULL;
	start = token;
	tmp = token;
	while (tmp)
    {
		while (tmp && tmp->type != TK_PIPE_1)
			tmp = tmp->next;
		new = ft_create_node(start);
		if (!new)
			return (ft_free_nodes(head));
		new->argv = ft_build_argv(start, tmp);
		if (!new->argv)
		{
			free(new);
			return (ft_free_nodes(head));
		}
		if (!head)
			head = new;
		else
			tail->next = new;
		tail = new;
		if (tmp && tmp->type == TK_PIPE_1)
		{
			start = tmp->next;
			tmp = tmp->next;
		}
	}
	return (head);
} */


t_node	*ft_node(t_token *token)
{
	t_token *start;
	t_token *tmp;
	t_node *new;
	t_node *head;

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
		ft_create_node(&head,new);
		ft_advance_tokens(&start, &tmp);
	}
	return (head);
}

t_node	*ft_get_node(t_token *token)
{
	t_node *new;
	t_token *tmp;

	new = malloc(sizeof(t_node));
	if (!new)
		return NULL;
	new->argv = NULL;
	new->redirs = NULL;
	new->next = NULL;
	tmp = token;
	while (tmp && tmp->type != TK_PIPE_1)
	{
		if (tmp->type >= TK_REDIR_IN_2 && tmp->type <= TK_HEREDOC_5)
		{
			ft_add_redirection(new, tmp);
			if (tmp->next) // Salta il token del filename dopo la redirezione
				tmp = tmp->next;
		}
		tmp = tmp->next;
	}
	return (new);
}

void	ft_create_node(t_node **head, t_node *new)
{
	t_node *tail;

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

void	ft_add_redirection(t_node *node, t_token *token)
{
	t_redir *new_redir; //t_node fd in base a cosa c'e nella redir del nodo
	t_redir *last;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return; 
	new_redir->type = token->type;
	new_redir->fd = -1; //di default, inizializza a -1
	new_redir->next = NULL;
	if (token->next && (token->next->type == TK_WORD_0 || token->next->type == TK_S_QUOTE_6 || token->next->type == TK_D_QUOTE_7 || token->next->type == TK_DOLLAR_8)) // Imposta il filename (se presente)
			new_redir->filename = ft_strdup(token->next->value);
	else
		new_redir->filename = NULL;
	if (!node->redirs) // Aggiungi alla lista delle redirezioni
		node->redirs = new_redir;
	else
	{
		last = node->redirs;
		while (last->next)
			last = last->next;
		last->next = new_redir;
	}
}

// void	ft_add_redirection(t_node *node, t_token *token)
// {
// 	t_redir *new_redir;
// 	t_redir *last;

// 	new_redir = malloc(sizeof(t_redir));
// 	if (!new_redir)
// 		return; 
// 	new_redir->type = token->type;
// 	new_redir->next = NULL;
// 	if (token->next && (token->next->type == TK_WORD_0 || token->next->type == TK_S_QUOTE_6 || token->next->type == TK_D_QUOTE_7 || token->next->type == TK_DOLLAR_8)) // Imposta il filename (se presente)
// 		{
// 			new_redir->filename = ft_strdup(token->next->value);
// 			new_redir->fd = open(new_redir->filename, O_RDONLY);
// 		}
// 	else
// 		new_redir->filename = NULL;
// 	if (!node->redirs) // Aggiungi alla lista delle redirezioni
// 		node->redirs = new_redir;
// 	else
// 	{
// 		last = node->redirs;
// 		while (last->next)
// 			last = last->next;
// 		last->next = new_redir;
// 	}
// }