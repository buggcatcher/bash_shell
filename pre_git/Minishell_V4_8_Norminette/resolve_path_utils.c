/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve_path_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/30 19:03:10 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_invalid_cmd(char *cmd)
{
	if (cmd[0] == '\0')
	{
		write(2, "command not found\n", 19);
		return (true);
	}
	return (false);
}

bool	is_absolute_path(char *cmd)
{
	return (ft_strchr(cmd, '/') != NULL);
}

char	**get_paths(t_env *env)
{
	char	*path_var;

	path_var = get_env_value("PATH", env);
	if (!path_var)
		return (NULL);
	return (ft_split(path_var, ':'));
}
