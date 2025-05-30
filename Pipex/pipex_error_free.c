/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_error_free.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 19:51:41 by vloddo            #+#    #+#             */
/*   Updated: 2025/03/10 19:51:43 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_check(t_pipex *pipex, int free_level, char *msg, int exit_code)
{
	free_memory(pipex, free_level);
	close_data(pipex);
	ft_error(msg, exit_code);
}

void	close_data(t_pipex *data)
{
	if (data->infd)
		close(data->infd);
	if (data->outfd)
		close(data->outfd);
	if (data->pipe_fd[0])
		close(data->pipe_fd[0]);
	if (data->pipe_fd[0])
		close(data->pipe_fd[1]);
	close(0);
	close(1);
}

void	ft_free_split(char **split_array)
{
	int	i;

	if (!split_array)
		return ;
	i = 0;
	while (split_array[i] != NULL)
	{
		free(split_array[i]);
		i++;
	}
	free(split_array);
}

void	free_memory(t_pipex *data, int i) //bellino
{
	while (i <= 5)
	{
		if (i == 5)
			ft_free_split(data->paths);
		else if (i == 4)
			ft_free_split(data->arg_cmd);
		else if (i == 3)
			free(data->cmd);
		else if (i == 2)
			ft_free_split(data->arg_cmd2);
		else if (i == 1)
			free(data->cmd2);
		i++;
	}
}

void	ft_error(char *str, int error)
{
	if (error == 2)
		write(2, "Usage: ./pipex file1 cmd1 cmd2 file2\n", 37);
	else if (error == 3)
		write(2, "zsh: Command not found\n", 24);
	else if (error == 4)
		write(2, "zsh: Path not set\n", 19);
	else if (error == 5)
		write(2, "zsh: Is a directory\n", 24);
	else if (error == 6)
		write(2, "zsh: Fork failed\n", 18);
	else
	{
		perror(str);
		exit(error);
	}
	exit(EXIT_FAILURE);
}
