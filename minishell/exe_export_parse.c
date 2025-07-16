/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_export_parse.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/07/16 17:50:42 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_numeric_char(char c)
{
	return (c >= '0' && c <= '9');
}

static bool	is_underscore(char c)
{
	return (c == '_');
}

static bool	is_alpha_char(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

bool	is_valid_variable_name(const char *str)
{
	int	i;

	if (!str || (!is_alpha_char(str[0]) && !is_underscore(str[0])))
		return (false);
	i = 1;
	while (str[i])
	{
		if (!is_alpha_char(str[i]) && !is_numeric_char(str[i]) && !is_underscore(str[i]) && (str[i] >= ' ' && str[i] <= '\0'))
			return (false);
		i++;
	}
	if ((str[i]) == '\0' && (str[i - 1]) == '+')
		return (false);
	return (true);
}

void	print_export_error(char *arg)
{
	ft_putstr_stderr("export: '");
	ft_putstr_stderr(arg);
	ft_putstr_stderr("': not a valid identifier\n");
}
