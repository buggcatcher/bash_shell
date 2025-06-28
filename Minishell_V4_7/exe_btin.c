/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_btin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/28 17:30:45 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exe_pwd(char **args, t_env **env)
{
	char	cwd[PATH_MAX + 1];

	(void)args;
	(void)env;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return (0);
	}
	write(2, "pwd\n", 5);
	return (1);
}

int	exe_unset(char **args, t_env **env)
{
	int	i;
	int	exit_status;

	i = 1;
	exit_status = 0;
	if (!args[1])
		return (write(2, "unset: not enough arguments\n", 28), 1);
	while (args[i])
	{
		if (remove_env_node(env, args[i]) != 0)
			write(2, "unset: remove node failed\n", 27);
		i++;
	}
	return (exit_status);
}

int	exe_env(t_env *env)
{
	while (env)
	{
		if (env->exported)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}

// int	exe_echo(char **args)
// {
// 	int	i;
// 	int	no_newline;

// 	i = 1;
// 	no_newline = 0;
// 	if (args[1] && ft_strcmp(args[1], "-n") == 0)
// 	{
// 		no_newline = 1;
// 		i = 2;
// 	}
// 	while (args[i])
// 	{
// 		printf("%s", args[i]);
// 		if (args[i + 1])
// 			printf(" ");
// 		i++;
// 	}
// 	if (!no_newline)
// 		printf("\n");
// 	return (0);
// }

static int	ft_echo_flag(char **str)
{
	int	i;

	i = 0;
	if (str == NULL || str[0] != '-')
		return (1);
	int i = 1;
	if (str[i] == '\0')
		return (1);
	while (str[i]) 
	{
		if (str[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

int	exe_echo(char **args)
{
	int	i;
	int	no_newline;

	i = 1;
	no_newline = 0;
	if (args[1] && ft_echo_flag(args[1]) == 0)
	{
		no_newline = 1;
		i = 2;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (!no_newline)
		printf("\n");
	return (0);
}
