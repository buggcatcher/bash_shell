/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/07/07 19:01:47 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_invalid_cmd(char *cmd)
{
	if (cmd[0] == '\0')
	{
		write(2, "command not found\n", 19);
		return (true);
	}
	return (false);
}

static bool	is_absolute_path(char *cmd)
{
	return (ft_strchr(cmd, '/') != NULL);
}

static char	**get_paths(t_env *env)
{
	char	*path_var;

	path_var = get_env_value("PATH", env);
	if (!path_var)
		return (NULL);
	return (ft_split(path_var, ':'));
}

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

// char	*resolve_path(char *cmd, t_env *env)
// {
// 	char	**paths;
// 	char	*full_path;

// 	if (is_invalid_cmd(cmd))
// 		return (NULL);
// 	if (is_absolute_path(cmd))
// 		return (ft_strdup(cmd));
// 	paths = get_paths(env);
// 	if (!paths)
// 		return (NULL);
// 	full_path = search_in_paths(paths, cmd);
// 	free_split_all(paths);
// 	if (!full_path)
// 		write(2, "command not found\n", 19);
// 	return (full_path);
// }

// static int	has_heredoc_before_redirect_out(t_redir *redir_list)
// {
// 	t_redir	*current;
// 	int		seen_heredoc = 0;

// 	current = redir_list;
// 	while (current)
// 	{
// 		if (current->type == TK_HEREDOC_5)
// 			seen_heredoc = 1;
// 		else if ((current->type == TK_REDIR_OUT_3 || current->type == TK_REDIR_OUT_3) && seen_heredoc)
// 			return (1);
// 		current = current->next;
// 	}
// 	return (0);
// }

// static int	has_heredoc_before_redirect(t_redir *redir_list)
// {
// 	t_redir	*current;
// 	t_redir	*last;

// 	current = redir_list;
// 	last = NULL;
// 	while (current)
// 	{
// 		last->type = current->type;
// 		current = current->next;
// 	}
// 	if (last->type == TK_HEREDOC_5)
// 		return (1);
// 	return (0);
// }

// char	*resolve_path(char *cmd, t_env *env, t_node *node)
// {
// 	char	**paths;
// 	char	*full_path;

// 	if (is_invalid_cmd(cmd))
// 		return (NULL);
// 	if (is_absolute_path(cmd))
// 		return (ft_strdup(cmd));
// 	paths = get_paths(env);
// 	if (!paths)
// 		return (NULL);
// 	full_path = search_in_paths(paths, cmd);
// 	free_split_all(paths);
// 	if (!full_path && !has_heredoc_before_redirect_out(node->redirs))
// 		write(2, "command not found\n", 19);
// 	return (full_path);
// }

//NEW
static int	has_heredoc_before_redirect_out(t_redir *redir_list)
{
	t_redir	*current;

	current = redir_list;
	while (current)
	{
		if (current->type == TK_HEREDOC_5 && current->next != NULL)
			return (1);
			
		current = current->next;
	}
	return (0);
}

static void ft_free_cmd_not_found(t_env *env, t_node *node)
{
	ft_free_nodes(node);
	ft_free_env(env);
	
}

char	*resolve_path(char *cmd, t_env *env, t_node *node)
{
	char	**paths;
	char	*full_path;

	if (is_invalid_cmd(cmd))
		return (ft_free_cmd_not_found(env, node), NULL);
	if (is_absolute_path(cmd))
		return (ft_strdup(cmd));
	paths = get_paths(env);
	if (!paths)
		return (NULL);
	full_path = search_in_paths(paths, cmd);
	free_split_all(paths);
	if (!full_path && !has_heredoc_before_redirect_out(node->redirs))
	{
		ft_free_cmd_not_found(env, node);
		write(2, "command not found\n", 19);
		exit(1); // NEW aggiunto exit per evitare double leack in caso di command not found 
	}
		return (full_path);
}