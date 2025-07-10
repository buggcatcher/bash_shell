/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/07/10 16:48:01 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int exec_in_parent(t_node *node, t_shell_state *state)
{
    int save_out;
    int save_in;
    
    printf("=== EXEC_IN_PARENT DEBUG ===\n");
    printf("node->argv = %p\n", node->argv);
    if (node->argv)
       printf("node->argv[0] = %s\n", node->argv[0] ? node->argv[0] : "(null)");
    save_out = save_fd(STDOUT_FILENO);
    printf("save_out = %d\n", save_out);
    save_in = save_fd(STDIN_FILENO);
    printf("save_in = %d\n", save_in);
    if (!node->argv || !node->argv[0])
    {
        printf("Entering empty command branch\n");
        if (node->redirs && apply_redirects(node->redirs))
        {
            printf("Redirects failed\n");
            state->last_status = 1;
        }
        else
        {
            printf("Redirects OK\n");
            state->last_status = 0;
        }
        printf("About to restore FDs\n");
    }
    else if (!strcmp(node->argv[0], "exit"))
    {
        printf("Executing exit\n");
        exe_exit(node->argv, state);
        return (state->last_status);
    }
    else
    {
        printf("Executing builtin\n");
        if (node->redirs && apply_redirects(node->redirs))
            state->last_status = 1;
        else
            state->last_status = exec_builtin(node->argv, &state->env);
    }
    printf("About to switch_fd restore\n");
    switch_fd(save_out, STDOUT_FILENO);
    switch_fd(save_in, STDIN_FILENO);
    printf("About to close FDs\n");
    close(save_out);
    close(save_in);
    printf("Returning from exec_in_parent\n");
    return (state->last_status);
}

void	update_fds_for_next_cmd(t_node *node, int pipe_fd[2], int *prev_fd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (node->next)
	{
		*prev_fd = pipe_fd[0];
		close(pipe_fd[1]);
	}
}

int	wait_for_last(pid_t last_pid, int *exit_status)
{
	int		status;
	pid_t	pid;
	int		signal;

	while (1)
	{
		pid = wait(&status);
		if (pid == -1)
			break ;
		if (pid == 0)
			continue ;
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				*exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				signal = WTERMSIG(status);
				*exit_status = 128 + signal;
				if (signal == SIGINT || signal == SIGQUIT)
					ft_putstr_stderr("\n");
			}
		}
	}
	return (*exit_status);
}

int	executor_loop(t_node *node, t_shell_state *state)
{
	pid_t	last_pid;
	pid_t	pid = 0;
    //t_node  *original_node;
	int		prev_fd;
	int		pipe_fd[2];
    
	last_pid = -1;
	prev_fd = -1;
    //original_node = node;
    while (node)
    {
        printf("====== 🟢 EXECUTOR DEBUG 🟢 ======\n");
        printf("node->argv[0] = %s\n", node->argv[0] ? node->argv[0] : "(null)");
        printf("node->redirs = %p\n", node->redirs);
        if (node->redirs)
           printf("first redir type = %d, fd = %d\n", node->redirs->type, node->redirs->fd);
        debug_print_nodes(node);
        if (create_pipe_if_needed(node, pipe_fd))
            return (1);
        printf("should_execute_in_parent = %d\n", should_execute_in_parent(node));
        if (should_execute_in_parent(node))
        {
            printf("EXECUTING IN PARENT\n");
            return (exec_in_parent(node, state));
        }
        printf("FORKING...\n");
		pid = fork();
		if (pid == -1)
			return (write(2, "Fork\n", 6), 1);
		if (pid == 0) {
			printf("EXECUTIN IN CHILD\n");
			exec_child(node, pipe_fd, prev_fd, state->env); // qui
		}
		else
			last_pid = pid;
		update_fds_for_next_cmd(node, pipe_fd, &prev_fd);
		node = node->next;
	}
	return (wait_for_last(last_pid, &state->last_status));
}
