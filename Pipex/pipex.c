/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:28:47 by vloddo            #+#    #+#             */
/*   Updated: 2025/02/28 13:28:50 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	first_child_process(t_pipex pipex, char **argv, char **envp)
{
	close(pipex.pipe_fd[0]);
	pipex.infd = open(argv[1], O_DIRECTORY);
	if (pipex.infd != -1)
		error_check(&pipex, 5, "directory", 5);
	pipex.infd = open(argv[1], O_RDONLY);
	if (pipex.infd == -1)
	{
		ft_free_split(pipex.paths);
		close(pipex.pipe_fd[1]);
		perror("zsh");
		exit(EXIT_FAILURE);
	}
	if (dup2(pipex.pipe_fd[1], STDOUT_FILENO) == -1 || \
		dup2(pipex.infd, STDIN_FILENO) == -1)
		error_check(&pipex, 5, "zsh", 1);
	close(pipex.pipe_fd[1]);
	close(pipex.infd);
	pipex.arg_cmd = ft_split(argv[2], ' ');
	if (!pipex.arg_cmd || !pipex.arg_cmd[0])
		error_check(&pipex, 4, "Cmd", 3);
	pipex.cmd = check_args(pipex.paths, pipex.arg_cmd[0]);
	if (!pipex.cmd)
		error_check(&pipex, 3, "Cmd", 3);
	execve(pipex.cmd, pipex.arg_cmd, envp);
	error_check(&pipex, 3, "zsh", 1);
}

static void	second_child_process(t_pipex pipex, char **argv, char **envp)
{
	close(pipex.pipe_fd[1]);
	pipex.outfd = open(argv[4], O_DIRECTORY);
	if (pipex.outfd != -1)
		error_check(&pipex, 5, "directory", 5);
	pipex.outfd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex.outfd == -1)
	{
		ft_free_split(pipex.paths);
		close(pipex.pipe_fd[0]);
		perror("zsh");
		exit(EXIT_FAILURE);
	}
	if (dup2(pipex.pipe_fd[0], STDIN_FILENO) == -1 || \
		dup2(pipex.outfd, STDOUT_FILENO) == -1)
		error_check(&pipex, 5, "zsh", 1);
	close(pipex.pipe_fd[0]);
	close(pipex.outfd);
	pipex.arg_cmd2 = ft_split(argv[3], ' ');
	if (!pipex.arg_cmd2 || !pipex.arg_cmd2[0])
		error_check(&pipex, 2, "Cmd", 3);
	pipex.cmd2 = check_args(pipex.paths, pipex.arg_cmd2[0]);
	if (!pipex.cmd2)
		error_check(&pipex, 1, "Cmd", 3);
	execve(pipex.cmd2, pipex.arg_cmd2, envp);
	error_check(&pipex, 5, "zsh", 1);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	pipex = (t_pipex){0};
	if (argc != 5)
		ft_error("Main", 2);
	pipex.paths = ft_get_path(envp);
	if (pipe(pipex.pipe_fd) == -1)
		error_check(&pipex, 5, "zsh", 1);
	pipex.p_id = fork();
	if (pipex.p_id == -1)
		error_check(&pipex, 5, "fork", 6);
	if (pipex.p_id == 0)
		first_child_process(pipex, argv, envp);
	pipex.p_id_2 = fork();
	if (pipex.p_id_2 == -1)
		error_check(&pipex, 3, "fork", 6);
	if (pipex.p_id_2 == 0)
		second_child_process(pipex, argv, envp);
	close_data(&pipex);
	waitpid(pipex.p_id, NULL, 0);
	waitpid(pipex.p_id_2, NULL, 0);
	free_memory(&pipex, 1);
	return (0);
}
