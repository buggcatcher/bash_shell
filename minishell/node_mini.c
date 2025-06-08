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
	t_token *cmd_start;
	t_token *tmp;
	t_node *new;

	head = NULL;
	tail = NULL;
	cmd_start = token;
	tmp = token;
	while (tmp)
    {
		while (tmp && tmp->type != TK_PIPE_1)
			tmp = tmp->next;
		new = ft_create_node(cmd_start);
		if (!new)
			return (ft_free_nodes(head));
		new->argv = ft_build_argv(cmd_start, tmp);
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
			cmd_start = tmp->next;
			tmp = tmp->next;
		}
	}
	return (head);
} */


t_node	*ft_node(t_token *token)
{
	t_token *cmd_start;
	t_token *tmp;
	t_node *new;
	t_node *head;

	head = NULL;
	cmd_start = token;
	tmp = token;
	while (tmp)
    {
		while (tmp && tmp->type != TK_PIPE_1)
			tmp = tmp->next;
		new = ft_get_node(cmd_start);
		if (!new)
			return (ft_free_nodes(head));
		new->argv = ft_build_argv(cmd_start, tmp);
		if (!new->argv)
		{
			free(new);
			return (ft_free_nodes(head));
		}
		ft_create_node(&head,new);
		ft_advance_tokens(&cmd_start, &tmp);
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

void	ft_advance_tokens(t_token **cmd_start, t_token **tmp)
{
	if (*tmp && (*tmp)->type == TK_PIPE_1)
	{
		*cmd_start = (*tmp)->next;
		*tmp = (*tmp)->next;
	}
}

void	ft_add_redirection(t_node *node, t_token *token)
{
	t_redir *new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return; 
	new_redir->type = token->type;
	new_redir->fd = -1; //di default
	new_redir->next = NULL;
	if (token->next && (token->next->type == TK_WORD_0 || token->next->type == TK_S_QUOTE_6 || token->next->type == TK_D_QUOTE_7 || token->next->type == TK_DOLLAR_8)) // Imposta il filename (se presente)
		new_redir->filename = ft_strdup(token->next->value);
	else
		new_redir->filename = NULL;
	if (!node->redirs) // Aggiungi alla lista delle redirezioni
		node->redirs = new_redir;
	else
	{
		t_redir *last = node->redirs;
		while (last->next)
			last = last->next;
		last->next = new_redir;
	}
}

/*
char	**ft_build_argv(t_token *start, t_token *end)
{
	t_token *tmp;
	char **argv;
	int i;
	int	j;

	tmp = start;
	j = 0;
	while (tmp != end && tmp) // Conta i token di tipo WORD (compresi quote e dollar)
	{
		if (tmp->type == TK_WORD_0 || tmp->type == TK_S_QUOTE_6 || tmp->type == TK_D_QUOTE_7 || tmp->type == TK_DOLLAR_8)
			j++;
		tmp = tmp->next;
	}
	argv = malloc((j + 1) * sizeof(char *)); // Alloca l'array di stringhe
	if (!argv)
		return (NULL);
	i = 0; // Popola l'array
	tmp = start;
	while (tmp != end && tmp)
	{
		if (tmp->type == TK_WORD_0 || tmp->type == TK_S_QUOTE_6 || tmp->type == TK_D_QUOTE_7 || tmp->type == TK_DOLLAR_8)
		{
			argv[i] = ft_strdup(tmp->value);
			if (!argv[i])
			{
				while (i-- > 0) // Libera memoria in caso di errore
					free(argv[i]);
				free(argv);
				return (NULL);
			}
			i++;
		}
		tmp = tmp->next;
	}
	argv[i] = NULL;
	return (argv);
} */

char	**ft_build_argv(t_token *start, t_token *end)
{
	char	**argv;
	int		j;

	j = ft_count_tokens(start, end);
	argv = malloc((j + 1) * sizeof(char *));
	if (!argv)
		return (NULL);
	argv = populate_argv(argv, start, end);
	return (argv);
}


int	ft_count_tokens(t_token *start, t_token *end)
{
	t_token	*tmp;
	int		count;

	tmp = start;
	count = 0;
	while (tmp != end && tmp)
	{
		if (tmp->type == TK_WORD_0 || tmp->type == TK_S_QUOTE_6
			|| tmp->type == TK_D_QUOTE_7 || tmp->type == TK_DOLLAR_8)
			count++;
		tmp = tmp->next;
	}
	return (count);
}

char	**populate_argv(char **argv, t_token *start, t_token *end)
{
	t_token	*tmp;
	int		i;

	i = 0;
	tmp = start;
	while (tmp != end && tmp)
	{
		if (tmp->type == TK_WORD_0 || tmp->type == TK_S_QUOTE_6 || 
			tmp->type == TK_D_QUOTE_7 || tmp->type == TK_DOLLAR_8)
		{
			argv[i] = ft_strdup(tmp->value);
			if (!argv[i])
			{
				while (i-- > 0)
					free(argv[i]);
				free(argv);
				return (NULL);
			}
			i++;
		}
		tmp = tmp->next;
	}
	argv[i] = NULL;
	return (argv);
}

/*
t_node	*ft_free_nodes(t_node *head)
{
	t_node *tmp;
	t_redir *redir;
	t_redir *next_redir;
	int		i;

	while (head)
	{
		tmp = head;
		head = head->next;
		if (tmp->argv) // libera argv
		{
			i = 0;
			while (tmp->argv[i])
			{
				free(tmp->argv[i]);
				i++;
			}
			free(tmp->argv);
		}
		redir = tmp->redirs;
		while (redir) // libera redirect
		{
			next_redir = redir->next;
			free(redir->filename);
			free(redir);
			redir = next_redir;
		}
		free(tmp);
	}
	return (NULL);
} */

t_node	*ft_free_nodes(t_node *head)
{
	t_node	*tmp;

	while (head)
	{
		tmp = head;
		head = head->next;
		ft_free_argv(tmp->argv);
		ft_free_redirs(tmp->redirs);
		free(tmp);
	}
	return (NULL);
}

void	ft_free_argv(char **argv)
{
	int	i;

	if (!argv)
		return;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

void	ft_free_redirs(t_redir *redir)
{
	t_redir	*next_redir;

	while (redir)
	{
		next_redir = redir->next;
		free(redir->filename);
		free(redir);
		redir = next_redir;
	}
}

