/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/07/17 15:32:09 by vloddo           ###   ########.fr       */
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

static void	handle_redirections(t_node *node, t_env *env, t_node *head)
{
	//printf("Prima di apply_redirects, redir=%p\n", node->redirs);
	int i = apply_redirects(node->redirs); // qui
	if (node->redirs && i)
	{
		//printf("Primo redir: type=%d, fd=%d\n", node->redirs->type, node->redirs->fd);
		printf("HANDLE REDIRECTIONS\n");
		ft_free_token(node->token);
		ft_free_nodes(head);
		ft_free_env(env);
		//printf("Error in redirections\n");
		exit(1);
	}
	//printf("redirect is %d\n", i);
}

static void	handle_builtin(t_node *node, t_env **env)
{
	int	status;
	
	if (is_builtin(node->argv[0]))
	{
        //printf("is builtin\n");
		status = exec_builtin(node->argv, env);
		clean_exit(node, *env, status);
	}
}

static void	execute_command(t_node *node, t_env *env, t_node *head)
{
	char	*bin;
	char	**env_arr;

	if (!node->argv || !node->argv[0])
	{
		//printf("free_nodes in execute_command\n");
		ft_free_token(node->token);
		ft_free_nodes(node);
		exit(127);
	}
	if (ft_strchr(node->argv[0], '/'))
		bin = node->argv[0];
	else if (node->argv[0][0] == '.' && node->argv[0][1] == '\0')
	{
		write(2, "Error_2 in execute_command\n", 28);
		ft_free_token(node->token);
		ft_free_nodes(head);
		ft_free_env(env);
		exit(127);
	}
	else
		bin = resolve_path(node->argv[0], env, node, head);
	if (!bin)
	{
		ft_free_token(node->token);
		ft_free_nodes(node);
		ft_free_env(env);
		exit(127);
	}
	env_arr = env_to_array(env);
	execve(bin, node->argv, env_arr);
	write(2, "Error_1 in execute_command\n", 28);
	ft_free_token(node->token);
	free_array(env_arr);
	ft_free_nodes(node);
	ft_free_env(env);
	exit(127);
}

void	exec_child(t_node *node, int pipe_out[2], int pipe_in, t_env *env, t_node *head)
{
	disable_signals();
	if (node->next)
		switch_fd(pipe_out[1], STDOUT_FILENO);
	handle_pipes(pipe_in, pipe_out);
	handle_redirections(node, env, head);
	if (!node->argv || !node->argv[0])
	{
		ft_free_nodes(node);
		ft_free_env(env);
        //printf("nessun comando da eseguire\n");
		exit(0);
	}
	//printf("exec_chiild: node->argv[0] = %s\n", node->argv[0]);
	handle_builtin(node, &env);
	execute_command(node, env, head);
	//printf ("non ci arriva\n");
}
