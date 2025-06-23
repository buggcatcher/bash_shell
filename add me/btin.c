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

int	is_parent_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "cd") || \
			!ft_strcmp(cmd, "export") || \
			!ft_strcmp(cmd, "unset") || \
			!ft_strcmp(cmd, "exit"));
}

int	is_child_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "echo") || \
			!ft_strcmp(cmd, "pwd") || \
			!ft_strcmp(cmd, "env"));
}

int	should_execute_in_parent(t_node *node)
{
	return (is_parent_builtin(node->argv[0]) && !node->next);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (is_parent_builtin(cmd) || is_child_builtin(cmd));
}


