/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/26 16:03:54 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	switch_fd(int from, int to)
{
	if (from != to)
	{
		if (dup2(from, to) == -1)
		{
			perror("dup2");
			return (1);
		}
	}
	return (0);
}

int	save_fd(int fd)
{
	int	clone;

	clone = dup(fd);
	if (clone == -1)
	{
		perror("dup");
		exit(EXIT_FAILURE);
	}
	return (clone);
}

int	save_stdout(void)
{
	int	clone;

	clone = dup(1);
	if (clone == -1)
	{
		perror("dup stdout");
		exit(EXIT_FAILURE);
	}
	return (clone);
}

int	open_outfile( char *filename)
{
	int	fd;

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("error opening outfile");
		return (-1);
	}
	return (fd);
}
