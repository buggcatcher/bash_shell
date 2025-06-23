/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   system_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/23 19:18:05 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_node(t_env *node)
{
	if (node->key)
		free(node->key);
	if (node->value)
		free(node->value);
	free(node);
}

t_env	*create_env_node_m(char *env_entry)
{
	char	*equals_pos;
	t_env	*new_node;

	equals_pos = strchr(env_entry, '=');
	*equals_pos = '\0';
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup_m(env_entry);
	new_node->value = ft_strdup_m(equals_pos + 1);
	new_node->exported = 1;
	*equals_pos = '=';
	if (!new_node->key || !new_node->value)
	{
		cleanup_node(new_node);
		return (NULL);
	}
	return (new_node);
}

t_env	*process_env_entry(char *env_entry, t_env *env)
{
	char	*equals_pos;
	t_env	*new_node;

	equals_pos = strchr(env_entry, '=');
	if (!equals_pos)
		return (env);
	new_node = create_env_node_m(env_entry);
	if (!new_node)
		return (NULL);
	new_node->next = env;
	return (new_node);
}

t_env	*init_env_from_system(char **envp)
{
	t_env	*env;
	int		i;

	env = NULL;
	i = 0;
	while (envp && envp[i])
	{
		env = process_env_entry(envp[i], env);
		if (!env)
			return (NULL);
		i++;
	}
	return (env);
}

char	**env_to_array(t_env *env)
{
	int		count;
	char	**arr;
	int		i;

	count = env_size(env);
	arr = safe_alloc(count + 1, sizeof(char *), "env_to_array");
	if (!arr)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->exported)
		{
			arr[i] = join_key_value(env->key, env->value);
			if (!arr[i])
			{
				free_array(arr);
				return (NULL);
			}
			i++;
		}
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}
