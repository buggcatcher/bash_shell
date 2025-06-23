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

static void	handle_builtin(t_node *node, t_env **env)
{
	if (is_builtin(node->argv[0]))
		exit(exec_builtin(node->argv, env));
}

static void	execute_command(t_node *node, t_env *env)
{
	char	*bin;
	char	**env_arr;

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

void	exec_child(t_node *node, int pipe_out[2], int pipe_in, t_env *env)
{
	handle_pipes(pipe_in, pipe_out);
	if (node->next)
		switch_fd(pipe_out[1], STDOUT_FILENO);
	handle_redirections(node);
	handle_builtin(node, &env);
	execute_command(node, env);
}