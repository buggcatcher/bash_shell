/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/28 15:58:44 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sigint = 0;

char	*read_user_input(t_shell_state *state)
{
	char	*input;

	g_sigint = 0;
	input = readline("minishell$ ");
	if (!input)
	{
		printf("exit\n");
		state->should_exit = true;
		return (NULL);
	}
	if (input[0] == '\0' && g_sigint)
	{
		free(input);
		return (NULL);
	}
	if (input[0] != '\0')
		add_history(input);
	return (input);
}

bool	process_input(char *input, t_shell_state *state)
{
	t_token	*token;
	t_node	*node;

	token = ft_tokenize(state, NULL, input);
	if (!token)
		return (false);
	if (ft_check_syntax(token) == 1)
	{
		ft_free_token(token);
		return (false);
	}
	node = ft_node(token);
	ignore_signals();
	executor_loop(node, state);
	setup_signals();
	ft_free_token(token);
	ft_free_nodes(node);
	return (true);
}

void	shell_loop(t_shell_state *state)
{
	char	*input;

	while (!state->should_exit)
	{
		input = read_user_input(state);
		if (!input)
			continue ;
		if (!process_input(input, state))
		{
			free(input);
			continue ;
		}
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell_state	state;

	(void)argc;
	(void)argv;
	init_shell_state(&state, envp);
	setup_signals();
	shell_loop(&state);
	cleanup_shell_state(&state);
	rl_clear_history();
	return (state.exit_code);
}
