/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/29 16:54:51 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_pipes(int pipe_in, int pipe_out[2])
{
	if (pipe_in != -1)
		switch_fd(pipe_in, STDIN_FILENO);
	if (pipe_out[0] != -1)
		close(pipe_out[0]);
	if (pipe_out[1] != -1)
		close(pipe_out[1]);
	if (pipe_in != -1)
		close(pipe_in);
}

static void	handle_redirections(t_node *node)
{
	if (node->redirs && apply_redirects(node->redirs))
		exit(1);
}

// NEW
// static void	handle_redirections(t_node *node)
// {
// 	if (node->redirs && apply_redirects(node->redirs))
// 	{
// 		ft_free_nodes(node);
// 		exit(1);
// 	}
// }

static void	handle_builtin(t_node *node, t_env **env)
{
	if (is_builtin(node->argv[0]))
		exit(exec_builtin(node->argv, env));
}

// NEW
// static void	handle_builtin(t_node *node, t_env **env)
// {
// 	int	i;
	
// 	i = exec_builtin(node->argv, env);
// 	if (is_builtin(node->argv[0]))
// 	{
// 		ft_free_nodes(node);
// 		ft_free_env(*env);
// 		exit(i);
// 	}
// }

// static void	execute_command(t_node *node, t_env *env)
// {
// 	char	*bin;
// 	char	**env_arr;

// 	if (ft_strchr(node->argv[0], '/'))
// 		bin = node->argv[0];
// 	else
// 		bin = resolve_path(node->argv[0], env);
// 	if (!bin)
// 		exit(127);
// 	env_arr = env_to_array(env);
// 	execve(bin, node->argv, env_arr);
// 	write(2, "Execve\n", 8);
// 	free_array(env_arr);
// 	exit(127);
// }

//NEW 2
static void	execute_command(t_node *node, t_env *env)
{
	char	*bin;
	char	**env_arr;

	if (!node->argv || !node->argv[0])
		exit(127);
	if (ft_strchr(node->argv[0], '/'))
		bin = node->argv[0];
	else
		bin = resolve_path(node->argv[0], env);
	if (!bin)
		exit(127);
	env_arr = env_to_array(env);
	execve(bin, node->argv, env_arr);
	write(2, "Execve\n", 8);
	free_array(env_arr);
	exit(127);
}

// NEW
// static void	execute_command(t_node *node, t_env *env)
// {
// 	char	*bin;
// 	char	**env_arr;

// 	if (ft_strchr(node->argv[0], '/'))
// 		bin = node->argv[0];
// 	else
// 		bin = resolve_path(node->argv[0], env);
// 	if (!bin)
// 	{
// 		ft_free_nodes(node);
// 		ft_free_env(env);
// 		exit(127);
// 	}
// 	env_arr = env_to_array(env);
// 	execve(bin, node->argv, env_arr);
// 	write(2, "Execve\n", 8);
// 	free_array(env_arr);
// 	ft_free_nodes(node);
// 	ft_free_env(env);
// 	exit(127);
// }

void	exec_child(t_node *node, int pipe_out[2], int pipe_in, t_env *env)
{
	disable_signals();
	if (node->next)
		switch_fd(pipe_out[1], STDOUT_FILENO);
	handle_pipes(pipe_in, pipe_out);
	handle_redirections(node);
	handle_builtin(node, &env);
	execute_command(node, env);
}
