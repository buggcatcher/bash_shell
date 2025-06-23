/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/23 18:00:04 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_cwd_or_error(char *buf, const char *context)
{
	if (!getcwd(buf, PATH_MAX))
	{
		perror(context);
		return (1);
	}
	return (0);
}

char	*resolve_cd_target(char **args, t_env *env)
{
	char	*home;

	if (args[1])
		return (args[1]);
	home = get_env_value("HOME", env);
	if (!home)
	{
		write(2, "cd: HOME not set\n", 18);
		return (NULL);
	}
	return (home);
}

int	exe_cd(char **args, t_env **env)
{
	char	oldpwd[PATH_MAX];
	char	newpwd[PATH_MAX];
	char	*target;

	if (get_cwd_or_error(oldpwd, "cd getcwd (OLDPWD)"))
		return (1);
	target = resolve_cd_target(args, *env);
	if (!target)
		return (1);
	if (chdir(target) != 0)
	{
		perror("cd");
		return (1);
	}
	if (get_cwd_or_error(newpwd, "cd getcwd (PWD)"))
		return (1);
	set_env_var(env, "OLDPWD", oldpwd);
	set_env_var(env, "PWD", newpwd);
	return (0);
}
