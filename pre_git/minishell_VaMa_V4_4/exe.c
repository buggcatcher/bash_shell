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

char	*resolve_path(char *cmd, t_env *env)
{
	char	**paths;
	char	*path_var;
	char	*full_path;
	size_t	i;

	i = 0;
	if (cmd[0] == '\0')
		return (printf("command not found\n"), NULL);
	if (ft_strchr(cmd, '/')) 
		return (ft_strdup(cmd));
	path_var = get_env_value("PATH", env);
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	while (paths[i])
	{
		full_path = join_path(paths[i], cmd);
		if (!full_path)
			return (free_split_all(paths), NULL);
		if (access(full_path, X_OK) == 0)
			return (free_split_all(paths), full_path);
		free(full_path);
		i++;
	}
	free_split_all(paths);
	printf("command not found\n");
	return (NULL);
}

void	free_split_all(char **arr)
{
	size_t	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

char	*join_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full);
}

char	*get_env_value(char *key, t_env *env)
{
	if (!key)
		return (NULL);
	while (env) {
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

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

void	exec_child(t_node *node, int pipe_out[2], int pipe_in, t_env *env)
{
	char 	*bin;
	char	**env_arr;

	if (pipe_in != -1)
		switch_fd(pipe_in, STDIN_FILENO);
	if (node->next)
		switch_fd(pipe_out[1], STDOUT_FILENO);
	if (pipe_out[0] != -1)
		close(pipe_out[0]);
	if (pipe_out[1] != -1)
		close(pipe_out[1]);
	if (pipe_in != -1)
		close(pipe_in);
	if (node->redirs && apply_redirects(node->redirs))
		exit(1);
	if (is_builtin(node->argv[0]))
		exit(exec_builtin(node->argv, &env));
	
	if (ft_strchr(node->argv[0], '/'))
		bin = node->argv[0];
	else
		bin = resolve_path(node->argv[0], env);
	if (!bin)
		exit(127);
	env_arr = env_to_array(env);
	execve(bin, node->argv, env_arr);
	perror("execve");
	free_array(env_arr);
	exit(127);
}

int	spawn_forked_command(t_node *node, int pipe_fd[2], int prev_fd, t_env *env)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
		exec_child(node, pipe_fd, prev_fd, env);
	return (0);
}

char	**env_to_array(t_env *env)
{
	int	count;
	char **arr;
	int	i;

	count = env_size(env);
	arr = safe_alloc(count + 1, sizeof(char *), "env_to_array");
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->exported)
		{
			arr[i] = join_key_value(env->key, env->value);
			if (!arr[i])
			{
				free_array(arr);
				return (NULL);
			}
			i++;
		}
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}

int	exec_builtin(char **args, t_env **env)
{
	if (!args || !args[0])
		return (1);
	if (!ft_strcmp(args[0], "cd"))
		return exe_cd(args, env);
	else if (!ft_strcmp(args[0], "export"))
		return exe_export(args, env);
	else if (!ft_strcmp(args[0], "unset"))
		return exe_unset(args, env);
	else if (!ft_strcmp(args[0], "pwd"))
		return exe_pwd(args, env);
	else if (!ft_strcmp(args[0], "env"))
		return exe_env(*env);
	else if (!ft_strcmp(args[0], "echo"))
		return exe_echo(args);   
	return (1);
}

int	apply_redirects(t_redir *redirs)
{
	int	fd;
	int	target;

	while (redirs)
	{
		if (redirs->type == TK_REDIR_IN_2)
			fd = open(redirs->filename, O_RDONLY);
		else if (redirs->type == TK_REDIR_OUT_3)
			fd = open(redirs->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redirs->type == TK_REDIR_APPEND_4)
			fd = open(redirs->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redirs->type == TK_HEREDOC_5)
		{
			if (ft_handle_heredoc(redirs) != 0)
				return (1);
			fd = redirs->fd;  
		}
		else
			return (1);
		if (fd < 0)
			return (perror("redirect open error"), 1);
		if (redirs->type == TK_REDIR_IN_2 || redirs->type == TK_HEREDOC_5)
			target = STDIN_FILENO;
		else
			target = STDOUT_FILENO;
		if (switch_fd(fd, target) != 0)
		{
			close(fd);
			return (1);
		}
		close(fd);
		redirs = redirs->next;
	}
	return (0);
}

int	handle_builtin_if_applicable(t_node *node, t_env **env, int *last_status)
{
	if (is_builtin(node->argv[0]) && !node->next)
	{
		int	save_out;
		int	save_in;

		save_out = save_fd(STDOUT_FILENO);
		save_in = save_fd(STDIN_FILENO);
		if (node->redirs && apply_redirects(node->redirs))
			*last_status = 1;
		else
			*last_status = exec_builtin(node->argv, env);
		switch_fd(save_out, STDOUT_FILENO);
		switch_fd(save_in, STDIN_FILENO);
		close(save_out);
		close(save_in);
		return 1;
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

t_env	*init_env_from_system(char **envp)
{
	t_env	*env;
	t_env	*new_node;
	char	*equals_pos;
	int		i;

	env = NULL;
	i = 0;
	while (envp && envp[i])
	{
		equals_pos = strchr(envp[i], '=');
		if (equals_pos)
		{
			new_node = malloc(sizeof(t_env));
			if (!new_node)
				return (NULL);
			*equals_pos = '\0';
			new_node->key = ft_strdup_m(envp[i]);
			new_node->value = ft_strdup_m(equals_pos + 1);
			if (!new_node->key || !new_node->value)
			{
				if (new_node->key) free(new_node->key);
				if (new_node->value) free(new_node->value);
				free(new_node);
				free_env_list(env);
				*equals_pos = '=';
				return (NULL);
			}
			new_node->exported = 1;
			*equals_pos = '=';
			new_node->next = env;
			env = new_node;
		}
		i++;
	}
	return (env);
}

void	init_shell_state(t_shell_state *state, char **envp)
{
	state->env = init_env_from_system(envp);
	state->last_status = 0;
	state->should_exit = 0;
	state->exit_code = 0;
}

void	cleanup_shell_state(t_shell_state *state)
{
	t_env	*current;
	t_env	*next;

	current = state->env;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
	state->env = NULL;
}

int	executor_loop(t_node *node, t_shell_state *state)
{
	pid_t	last_pid;
	pid_t	pid;
	int		prev_fd;
	int		pipe_fd[2];
	int		save_out;
	int		save_in;
	int		status;

	prev_fd = -1;
	last_pid = -1;
	while (node)
	{
		if (create_pipe_if_needed(node, pipe_fd))
			return (1);
		if (should_execute_in_parent(node))
		{
			if (!strcmp(node->argv[0], "exit"))
			{
				exe_exit(node->argv, state);
				return (state->last_status);
			}
			save_out = save_fd(STDOUT_FILENO);
			save_in = save_fd(STDIN_FILENO);
			if (node->redirs && apply_redirects(node->redirs))
				state->last_status = 1;
			else
				state->last_status = exec_builtin(node->argv, &state->env);
			switch_fd(save_out, STDOUT_FILENO);
			switch_fd(save_in, STDIN_FILENO);
			close(save_out);
			close(save_in);
			return (state->last_status);
		}
		else
		{
			pid = fork();
			if (pid == -1)
				return (perror("fork"), 1);
			if (pid == 0)
				exec_child(node, pipe_fd, prev_fd, state->env);
			else
			last_pid = pid;
			if (prev_fd != -1)
				close(prev_fd);
			if (node->next)
			{
				prev_fd = pipe_fd[0];
				close(pipe_fd[1]);
			}
		}
		node = node->next;
	}
	while ((pid = wait(&status)) > 0)
	{
		if (pid == last_pid) {
			if (WIFEXITED(status))
				state->last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				state->last_status = 128 + WTERMSIG(status);
		}
	}
	return (state->last_status);
}