/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:28:58 by vloddo            #+#    #+#             */
/*   Updated: 2025/02/28 13:29:07 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && (s1[i] || s2[i]))
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

size_t	ft_strlen(const char *a)
{
	size_t	i;

	i = 0;
	while (a[i] != '\0')
		i++;
	return (i);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*final;
	int		i;
	int		j;

	i = ft_strlen(s1);
	j = ft_strlen(s2);
	final = malloc(sizeof(char) * (i + j + 1));
	if (!final)
		return (NULL);
	final[i + j] = '\0';
	while (j > 0)
	{
		j--;
		final[i + j] = s2[j];
	}
	while (i > 0)
	{
		i--;
		final[i] = s1[i];
	}
	return (final);
}

char	**ft_get_path(char **envp)
{
	int		i;
	char	**temp;

	i = 0;
	while (envp[i])
	{
		if ((ft_strncmp(envp[i], "PATH=", 5) == 0))
		{
			temp = ft_split((envp[i] + 5), ':');
			return (temp);
		}
		i++;
	}
	ft_error("Path", 4);
	return (NULL);
}

char	*check_args(char **paths, char *argcmd)
{
	char	*directory;
	char	*cmd;
	int		i;

	i = 0;
	while (paths[i])
	{
		directory = ft_strjoin(paths[i], "/");
		cmd = ft_strjoin(directory, argcmd);
		free(directory);
		if (access(cmd, X_OK) == 0)
		{
			return (cmd);
		}
		free(cmd);
		i++;
	}
	return (NULL);
}
