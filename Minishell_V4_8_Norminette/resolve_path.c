/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/30 18:58:42 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*search_in_paths(char **paths, char *cmd)
{
	size_t	i;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		full_path = join_path(paths[i], cmd);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

static int	has_heredoc_before_redirect(t_redir *redir_list)
{
	t_redir	*current;
	t_redir	*last;

	current = redir_list;
	last = NULL;
	while (current)
	{
		last->type = current->type;
		current = current->next;
	}
	if (last->type == TK_HEREDOC_5)
		return (1);
	return (0);
}

char	*resolve_path(char *cmd, t_env *env, t_node *node)
{
	char	**paths;
	char	*full_path;

	if (is_invalid_cmd(cmd))
		return (NULL);
	if (is_absolute_path(cmd))
		return (ft_strdup(cmd));
	paths = get_paths(env);
	if (!paths)
		return (NULL);
	full_path = search_in_paths(paths, cmd);
	free_split_all(paths);
	if (!full_path && !has_heredoc_before_redirect(node->redirs))
		write(2, "command not found\n", 19);
	return (full_path);
}
