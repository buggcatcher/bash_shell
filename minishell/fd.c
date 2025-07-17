/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/07/17 14:14:28 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	switch_fd(int src, int dest)
{
	if (src != dest)
	{
		if (dup2(src, dest) == -1)
		{
			write(2, "Dup2\n", 6);
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
		write(2, "Dup\n", 5);
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
		write(2, "Dup stdout\n", 12);
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
		write(2, "Error opening outfile\n", 23);
		return (-1);
	}
	return (fd);
}
