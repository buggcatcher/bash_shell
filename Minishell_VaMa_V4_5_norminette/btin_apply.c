/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btin_apply.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/23 19:21:24 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	memcpy(str, key, key_len);
	str[key_len] = '=';
	memcpy(str + key_len + 1, value, val_len);
	str[key_len + 1 + val_len] = '\0';
	return (str);
}

int	spawn_forked_command(t_node *n, int pipe_fd[2], int prev_fd, t_env *e)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
		exec_child(n, pipe_fd, prev_fd, e);
	return (0);
}

int	exec_builtin(char **args, t_env **env)
{
	if (!args || !args[0])
		return (1);
	if (!ft_strcmp(args[0], "cd"))
		return (exe_cd(args, env));
	else if (!ft_strcmp(args[0], "export"))
		return (exe_export(args, env));
	else if (!ft_strcmp(args[0], "unset"))
		return (exe_unset(args, env));
	else if (!ft_strcmp(args[0], "pwd"))
		return (exe_pwd(args, env));
	else if (!ft_strcmp(args[0], "env"))
		return (exe_env(*env));
	else if (!ft_strcmp(args[0], "echo"))
		return (exe_echo(args));
	return (1);
}

int	handle_builtin_if_applicable(t_node *n, t_env **e, int *last_status)
{
	int	save_out;
	int	save_in;

	if (is_builtin(n->argv[0]) && !n->next)
	{
		save_out = save_fd(STDOUT_FILENO);
		save_in = save_fd(STDIN_FILENO);
		if (n->redirs && apply_redirects(n->redirs))
			*last_status = 1;
		else
			*last_status = exec_builtin(n->argv, e);
		switch_fd(save_out, STDOUT_FILENO);
		switch_fd(save_in, STDIN_FILENO);
		close(save_out);
		close(save_in);
		return (1);
	}
	return (0);
}

int	create_pipe_if_needed(t_node *node, int pipe_fd[2])
{
	if (node->next)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
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
