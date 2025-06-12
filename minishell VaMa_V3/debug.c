#include "minishell.h"

void debug_status(int status)
{
	printf("[DEBUG] Exit status: %d\n", status);
}

void debug_pipe_test(t_node *node)
{
	if (!node || !node->next)
		return;

	printf("[DEBUG] cmd1 = %s\n", node->argv[0]);
	printf("[DEBUG] cmd2 = %s\n", node->next->argv[0]);
}

void debug_redirect_info(t_redir *redir)
{
	while (redir)
	{
		printf("Redirect type=%d, filename=%s\n", redir->type, redir->filename);
		redir = redir->next;
	}
}

void debug_print_node(t_node *node)
{
	int i = 0;
	if (!node)
		return;

	printf("=== [DEBUG] NODE ===\n");

	while (node->argv && node->argv[i])
	{
		printf("argv[%d]: %s\n", i, node->argv[i]);
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
		printf("[DEBUG] [Pipe %d] cmd: %s\n", i, head->argv[0]);
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

void debug_path_resolution(char *cmd, t_env *env)
{
    printf("[DEBUG] Resolving path for: %s\n", cmd);
    char *path_var = get_env_value("PATH", env);
    printf("[DEBUG] PATH = %s\n", path_var ? path_var : "NULL");

}