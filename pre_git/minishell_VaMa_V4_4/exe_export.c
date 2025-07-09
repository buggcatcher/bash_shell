/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/05/28 12:50:15 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_export_without_value(char *arg, t_env **env)
{
	t_env	*existing;
	t_env	*new_node;

	existing = find_env_node(*env, arg);
	if (existing)
		existing->exported = 1;
	else
	{
		new_node = create_env_node(arg, "", 1);
		if (new_node)
		{
			new_node->next = *env;
			*env = new_node;
		}
	}
}

void	handle_key_value_export(char *arg, char *equals, t_env **env)
{
	char	*key;
	char	*value;
	t_env	*existing;
	t_env	*new_node;

	*equals = '\0';
	key = arg;
	value = equals + 1;
	existing = find_env_node(*env, key);
	if (existing)
	{
		free(existing->value);
		existing->value = ft_strdup(value);
		existing->exported = 1;
	}
	else
	{
		new_node = create_env_node(key, value, 1);
		if (new_node)
		{
			new_node->next = *env;
			*env = new_node;
		}
	}
	*equals = '=';
}

void	handle_single_export(char *arg, t_env **env)
{
	char	*equals;

	equals = strchr(arg, '=');
	if (equals)
		handle_key_value_export(arg, equals, env);
	else
		handle_export_without_value(arg, env);
}

int	print_exported_env(t_env *env)
{
	while (env)
	{
		if (env->exported)
			printf("declare -x %s=\"%s\"\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}

int	exe_export(char **args, t_env **env)
{
	int	i;

	if (!args[1])
		return (print_exported_env(*env));
	i = 1;
	while (args[i])
	{
		handle_single_export(args[i], env);
		i++;
	}
	return (0);
}
