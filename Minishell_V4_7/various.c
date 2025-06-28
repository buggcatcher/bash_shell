/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   various.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/24 19:41:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_split_all(char **arr)
{
	size_t	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

char	*join_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full);
}

char	*get_env_value(char *key, t_env *env)
{
	if (!key)
		return (NULL);
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	init_shell_state(t_shell_state *state, char **envp)
{
	state->env = init_env_from_system(envp);
	state->last_status = 0;
	state->should_exit = 0;
	state->exit_code = 0;
}

void	cleanup_shell_state(t_shell_state *state)
{
	t_env	*current;
	t_env	*next;

	current = state->env;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
	state->env = NULL;
}
