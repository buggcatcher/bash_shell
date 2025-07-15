/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btin_apply.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/07/04 21:20:37 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	*join_key_value(char *key, char *value)
// {
// 	size_t	key_len;
// 	size_t	val_len;
// 	char	*str;

// 	if (!key || !value)
// 		return (NULL);
// 	key_len = strlen(key);
// 	val_len = strlen(value);
// 	str = safe_alloc(key_len + val_len + 2, sizeof(char), "join_key_value");
// 	if (!str)
// 		return (NULL);
// 	memcpy(str, key, key_len);
// 	str[key_len] = '=';
// 	memcpy(str + key_len + 1, value, val_len);
// 	str[key_len + 1 + val_len] = '\0';
// 	return (str);
// }

// NEW CON FUNZIONE FT_MEMCPY
char	*join_key_value(char *key, char *value)
{
	size_t	key_len;
	size_t	val_len;
	char	*str;

	if (!key || !value)
		return (NULL);
	key_len = strlen(key);
	val_len = strlen(value);
	str = safe_alloc(key_len + val_len + 2, sizeof(char), "join_key_value");
	if (!str)
		return (NULL);
	ft_memcpy(str, key, key_len);
	str[key_len] = '=';
	ft_memcpy(str + key_len + 1, value, val_len);
	str[key_len + 1 + val_len] = '\0';
	return (str);
}

// int	spawn_forked_command(t_node *node, int pipe_fd[2], int prev_fd, t_env *env)
// {
// 	pid_t	pid;

// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		write(2, "Fork\n", 6);
// 		return (1);
// 	}
// 	if (pid == 0)
// 		exec_child(node, pipe_fd, prev_fd, env);
// 	return (0);
// }

int	exec_builtin(char **args, t_env **env)
{
	if (!args || !args[0])
		return (1);
	if (!ft_strcmp(args[0], "cd"))
	{
		printf("🔷 executing cd built-in\n");
		return (exe_cd(args, env));
	}
	else if (!ft_strcmp(args[0], "export"))
	{
		printf("🔷 executing export built-in\n"); 
		return (exe_export(args, env));
	}
	else if (!ft_strcmp(args[0], "unset"))
	{
		printf("🔷 executing unset built-in\n");
		return (exe_unset(args, env));
	}
	else if (!ft_strcmp(args[0], "pwd"))
	{
		printf("🔷 executing pwd built-in\n");
		return (exe_pwd(args, env));
	}
	else if (!ft_strcmp(args[0], "env"))
	{
		printf("🔷 executing env built-in\n");
		return (exe_env(*env));
	}
	else if (!ft_strcmp(args[0], "echo"))
	{
		printf("🔷 executing echo built-in\n");
		return (exe_echo(args));
	}
	return (1);
}

// int	handle_builtin_if_applicable(t_node *node, t_env **env, int *last_status)
// {
// 	int	save_out;
// 	int	save_in;

// 	if (is_builtin(node->argv[0]) && !node->next)
// 	{
// 		save_out = save_fd(STDOUT_FILENO);
// 		save_in = save_fd(STDIN_FILENO);
// 		if (node->redirs && apply_redirects(node->redirs))
// 			*last_status = 1;
// 		else
// 			*last_status = exec_builtin(node->argv, env);
// 		switch_fd(save_out, STDOUT_FILENO);
// 		switch_fd(save_in, STDIN_FILENO);
// 		close(save_out);
// 		close(save_in);
// 		return (1);
// 	}
// 	return (0);
// }

int	create_pipe_if_needed(t_node *node, int pipe_fd[2])
{
	if (node->next)
	{
		if (pipe(pipe_fd) == -1)
		{
			write(2, "Pipe\n", 6);
			return (1);
		}
	}
	else
	{
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
	}
	return (0);
}
