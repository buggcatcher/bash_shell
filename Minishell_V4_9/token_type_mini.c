/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type_mini.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/07/01 19:36:40 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*ft_pipe(t_token **new, char **input)
{
	*new = ft_create_token(TK_PIPE_1, *input, 1);
	(*input)++;
	return (*new);
}

t_token	*ft_redher(t_token **new, char **input)
{
	if (*(*input + 1) == '<')
	{
		*new = ft_create_token(TK_HEREDOC_5, *input, 2);
		*input += 2;
	}
	else
	{
		*new = ft_create_token(TK_REDIR_IN_2, *input, 1);
		(*input)++;
	}
	return (*new);
}

t_token	*ft_redred(t_token **new, char **input)
{
	if (*(*input + 1) == '>')
	{
		*new = ft_create_token(TK_REDIR_APPEND_4, *input, 2);
		*input += 2;
	}
	else
	{
		*new = ft_create_token(TK_REDIR_OUT_3, *input, 1);
		(*input)++;
	}
	return (*new);
}

t_token	*ft_squote(t_token *token, t_token **new, char **input)
{
	char	*start;

	(void)token;
	(*input)++;
	start = *input;
	while (**input && **input != '\'')
		(*input)++;
	if (**input != '\'')
	{
		(*input)++;
		return (ft_putstr_stderr("Unclosed single quote\n"), NULL);
	}
	while (**input && **input != ' ')
		(*input)++;
	(*input)--;
	*new = ft_create_token(TK_S_QUOTE_6, start, *input - start);
	(*input)++;
	return (*new);
}

// t_token	*ft_dquote(t_shell_state *s, t_token *t, t_token **n, char **i)
// {
// 	char	*start;
// 	char	*buffer;
// 	int		var;

// 	start = *i;
// 	buffer = NULL;
// 	var = 1;
// 	start = ++(*i);
// 	if (ft_check_dquote(s, t, start) == 1)
// 		return (NULL);
// 	while (**i && **i != ' ')
// 	{
// 		if (var != 2)
// 			var = ft_check_var(i);
// 		buffer = ft_create_var(buffer, i, s);
// 	}
// 	(*i)--;
// 	if (var == 1)
// 		*n = ft_create_token(TK_D_QUOTE_7, start, *i - start);
// 	else if (var == 2)
// 		*n = ft_create_token(TK_DOLLAR_8, buffer, ft_strlen_v(buffer));
// 	if (buffer)
// 		free(buffer);
// 	(*i)++;
// 	return (*n);
// }

t_token	*ft_dquote(t_shell_state *state, t_token *token, t_token **new, char **input)
{
	char	*start;
	char	*buffer;
	int		var;

	start = *input;
	buffer = NULL;
	var = 1;
	(*input)++;
	start = *input;
	if (ft_check_dquote(state, token, start) == 1)
		return (NULL);
	while (**input && **input != '"')
	{
		if (var != 2)
			var = ft_check_var(input);
		buffer = ft_create_var(buffer, input, state);
	}
	if (var == 1)
		*new = ft_create_token(TK_D_QUOTE_7, start, *input - start);
	else if (var == 2)
		*new = ft_create_token(TK_DOLLAR_8, buffer, ft_strlen_v(buffer));
	if (buffer)
		free(buffer);
	(*input)++;
	return (*new);
}
