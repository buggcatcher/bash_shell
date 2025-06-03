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
	const char	*start;

	(*input)++;
	start = *input;
	while (**input && **input != '\'')
		(*input)++;
	if (**input != '\'')
	{
		(*input)++;
		return (ft_error(token, "Unclosed single quote"), NULL);
	}
	*new = ft_create_token(TK_S_QUOTE_6, start, *input - start);
	(*input)++;
	return (*new);
}

// t_token	*ft_dquote(t_token *token, t_token **new, char **input)
// {
// 	const char	*start;
// 	int			var;

// 	(*input)++;
// 	start = *input;
// 	while (**input && **input != '"')
// 		(*input)++;
// 	if (**input != '"')
// 	{
// 		(*input)++;
// 		return (ft_error(token, "Unclosed double quote"), NULL);
// 	}
// 	*new = ft_create_token(TK_D_QUOTE_7, start, *input - start);
// 	(*input)++;
// 	return (*new);
// }

t_token	*ft_dquote(t_token *token, t_token **new, char **input)
{
	const char	*start;
	char		*buffer;
	int			var;

	start = *input;
	buffer = NULL;
	(*input)++;
	start = *input;
	while (**input && **input != '"')
	{
			var = ft_check_var(input);
			buffer = ft_create_var(buffer, input);
	}
	if (**input != '"')
	{
		(*input)++;
		free(buffer);
		return (ft_error(token, "Unclosed double quote"), NULL);
	}
	if (var == 1)
		*new = ft_create_token(TK_D_QUOTE_7, start, *input - start);
	else if (var == 2)
	{
		*new = ft_create_token(TK_DOLLAR_8, buffer, ft_strlen(buffer));
		free(buffer);
	}
	(*input)++;
	return (*new);
}

