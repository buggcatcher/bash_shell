/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/29 16:57:55 by vloddo           ###   ########.fr       */
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

	token = NULL;
	node = NULL;
	token = ft_tokenize(state, NULL, input);
	if (!token)
		return (false);
	if (ft_check_syntax(token) == 1)
	{
		ft_free_token(token);
		return (false);
	}
	node = ft_node(token);
	if (preprocess_heredocs(node) != 0)
    {
        ft_free_nodes(node);
        return (false);
    }
	//debug_print_nodes(node);
	//printf("Before executor_loop\n");
    ignore_signals();
    executor_loop(node, state);
    //printf("After executor_loop\n");
    setup_signals();
    //printf("After setup_signals\n");
    ft_free_nodes(node);
    //printf("After ft_free_nodes\n");
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
