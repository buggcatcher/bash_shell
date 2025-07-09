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

int	exit_numeric_error(char *arg, t_shell_state *state)
{
	ft_putstr_stderr("exit: ");
	ft_putstr_stderr(arg);
	ft_putstr_stderr(": numeric argument required\n");
	printf("exit\n");
	state->should_exit = 1;
	state->exit_code = 2;
	return (2);
}

int	exit_too_many_args(t_shell_state *state)
{
	ft_putstr_stderr("exit: too many arguments\n");
	state->last_status = 1;
	return (1);
}

int	exe_exit(char **args, t_shell_state *state)
{
	int		exit_code;
	long	val;
	char	*endptr;

	exit_code = state->last_status;
	if (args[1])
	{
		errno = 0;
		val = ft_strtol(args[1], &endptr);
		if (*endptr != '\0' || errno == ERANGE)
			return (exit_numeric_error(args[1], state));
		else if (args[2])
			return (exit_too_many_args(state));
		else
			exit_code = (int)(val & 0xFF);
	}
	printf("exit\n");
	state->should_exit = 1;
	state->exit_code = exit_code;
	return (0);
}
