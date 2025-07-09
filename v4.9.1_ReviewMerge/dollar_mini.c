/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_mini.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/07/07 21:26:00 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_dquote(t_shell_state *state, t_token *token, char *start)
{
	int	i;

	(void)token;
	i = 0;
	while (start[i] && start[i] != '"')
		i++;
	if (start[i] != '"')
	{
		state->last_status = 2;
		ft_putstr_stderr("Unclosed double quote\n");
		return (1);
	}
	return (0);
}

/*int	ft_check_var(char **input)
{
	char	*tmp;
	int		start_pos;

	if (**input != '$')
		return (1);
	tmp = *input + 1;
	start_pos = 0;
	if (tmp[start_pos] == '?')
		return (2);
	while (tmp[start_pos] && \
			((tmp[start_pos] >= '0' && tmp[start_pos] <= '9') || \
			(tmp[start_pos] >= 'A' && tmp[start_pos] <= 'Z') || \
			(tmp[start_pos] >= 'a' && tmp[start_pos] <= 'z') || \
			tmp[start_pos] == '_'))
		start_pos++;
	if (start_pos == 0)
		return (1);
	else
		return (2);
}*/

int	ft_check_var(char **input)
{
	char	*tmp;
	int		start_pos;

	if (**input != '$')
		return (1);	
	tmp = *input + 1;
	start_pos = 0;
	if (tmp[start_pos] == '?')
		return (2);
	while (tmp[start_pos] && \
			((tmp[start_pos] >= '0' && tmp[start_pos] <= '9') || \
			(tmp[start_pos] >= 'A' && tmp[start_pos] <= 'Z') || \
			(tmp[start_pos] >= 'a' && tmp[start_pos] <= 'z') || \
			tmp[start_pos] == '_'))
		start_pos++;
	if (start_pos == 0)
		return (1);
	else
		return (2);
}

char	*ft_create_var(char *buffer, char **input, t_shell_state *state)
{
	char	*var;
	char	*tmp;

	if (**input == '$')
	{
		var = ft_expand_var(input, state);
		if (!buffer)
			buffer = var;
		else
		{
			tmp = ft_strjoin(buffer, var);
			free(buffer);
			free(var);
			buffer = tmp;
		}
	}
	else
	{
		buffer = ft_append_char(buffer, **input);
		(*input)++;
	}
	return (buffer);
}

char	*ft_expand_var(char **input, t_shell_state *state)
{
	char	*start;
	char	*var_name;
	char	*var_value;

	(*input)++;
	start = *input;
	if (**input == '?')
	{
		(*input)++;
		return (ft_itoa(state->last_status));
	}
	while (**input && ((**input >= '0' && **input <= '9') || \
			(**input >= 'A' && **input <= 'Z') || \
			(**input >= 'a' && **input <= 'z') || **input == '_'))
		(*input)++;
	if (*input == start)
		return (ft_strdup("$"));
	var_name = ft_strndup(start, *input - start);
	var_value = ft_my_getenv(var_name, state);
	free(var_name);
	if (!var_value)
		var_value = "";
	return (ft_strdup(var_value));
}

char	*ft_my_getenv(char *var_name, t_shell_state *state)
{
	t_env	*current;

	current = state->env;
	while (current)
	{
		if (current->key && ft_strcmp(current->key, var_name) == 0)
			return (current->value);
		current = current->next;
	}
	return (getenv(var_name));
}
