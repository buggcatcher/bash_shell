/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_mini.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/28 20:51:50 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// NEW
void	ft_free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}
// NEW new meow new
t_node	*ft_free_nodes(t_node *head)
{
	t_node	*tmp;
	tmp = head;
	ft_free_token(tmp->token);

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


void	ft_free_token(t_token *token)
{
	t_token	*tmp;

	while (token)
	{
		tmp = token;
		token = token->next;
		if (tmp->value)
			free(tmp->value);
		free(tmp);
	}
}

// t_node	*ft_free_nodes(t_node *head)
// {
// 	t_node	*tmp;

// 	while (head)
// 	{
// 		tmp = head;
// 		head = head->next;
// 		ft_free_argv(tmp->argv);
// 		ft_free_redirs(tmp->redirs);
// 		free(tmp);
// 	}
// 	return (NULL);
// }

void	ft_free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
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

void	free_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
