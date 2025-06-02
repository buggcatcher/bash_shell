#include "exe.h"

void debug_status(int status)
{
	printf("[DEBUG] Exit status: %d\n", status);
}

void debug_pipe_test(t_node *node)
{
	if (!node || !node->next)
		return;

	printf("[DEBUG] cmd1 = %s\n", node->cmd_argv[0]);
	printf("[DEBUG] cmd2 = %s\n", node->next->cmd_argv[0]);
}

void debug_redirect_info(t_redir *redir)
{
	while (redir)
	{
		printf("Redirect type=%d, file=%s\n", redir->type, redir->file);
		redir = redir->next;
	}
}

void debug_print_node(t_node *node)
{
	int i = 0;
	if (!node)
		return;

	printf("=== [DEBUG] NODE ===\n");

	while (node->cmd_argv && node->cmd_argv[i])
	{
		printf("argv[%d]: %s\n", i, node->cmd_argv[i]);
		i++;
	}

	if (node->next)
		printf("Has next: Yes\n");
	else
		printf("Has next: No\n");

	printf("============\n");
}

void debug_print_pipeline(t_node *head)
{
	int i = 0;
	while (head)
	{
		printf("[DEBUG] [Pipe %d] cmd: %s\n", i, head->cmd_argv[0]);
		head = head->next;
		i++;
	}
}

void debug_print_env(t_env *env)
{
	while (env)
	{
		printf("[DEBUG] ENV: %s=%s\n", env->key, env->value);
		env = env->next;
	}
}

void debug_print_array(char **arr)
{
	int i = 0;
	while (arr && arr[i])
	{
		printf("arr[%d]: %s\n", i, arr[i]);
		i++;
	}
}

void debug_execve_args(char *path, char **argv, char **env)
{
	printf("[DEBUG] execve path: %s\n", path);
	debug_print_array(argv);
	(void)env; // Ignora parametro non usato
}
