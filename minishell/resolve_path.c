/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/07/16 19:25:49 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_invalid_cmd(char *cmd)
{
	if (cmd == NULL || cmd[0] == '\0')
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
	if (node && node->token)
		ft_free_token(node->token);
	ft_free_nodes(node);
	ft_free_env(env);
}

char	*resolve_path(char *cmd, t_env *env, t_node *node, t_node *head)
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
	if (!full_path && !has_heredoc_before_redirect_out(node->redirs))
	{
		ft_free_cmd_not_found(env, head);
		write(2, "command not found\n", 19);
		//write(2, "sono entrato in (!full_path && !has_heredoc_before_redirect_out(node->redirs))\n", 80);
		//debug_print_nodes(node); // reverse ingeneerng
		//argv[0] deve darti invalid read e invece con debug_print_nodes vedo che esiste ancora
		exit(127); // NEW aggiunto exit per evitare double leack in caso di command not found 
	}
		return (full_path);
}
 