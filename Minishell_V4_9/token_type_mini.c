/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type_mini.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/07/02 17:40:55 by vloddo           ###   ########.fr       */
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

// t_token	*ft_squote(t_token *token, t_token **new, char **input)
// {
// 	char	*start;

// 	(void)token;
// 	(*input)++;
// 	start = *input;
// 	while (**input && **input != '\'')
// 		(*input)++;
// 	if (**input != '\'')
// 	{
// 		(*input)++;
// 		return (ft_putstr_stderr("Unclosed single quote\n"), NULL);
// 	}
// 	while (**input && **input != ' ')
// 		(*input)++;
// 	(*input)--;
// 	*new = ft_create_token(TK_S_QUOTE_6, start, *input - start);
// 	(*input)++;
// 	return (*new);
// }

// t_token	*ft_dquote(t_shell_state *state, t_token *token, t_token **new, char **input)
// {
// 	char	*start;
// 	char	*buffer;
// 	int		var;

// 	start = *input;
// 	buffer = NULL;
// 	var = 1;
// 	(*input)++;
// 	start = *input;
// 	if (ft_check_dquote(state, token, start) == 1)
// 		return (NULL);
// 	while (**input && **input != '"')
// 	{
// 		if (var != 2)
// 			var = ft_check_var(input);
// 		buffer = ft_create_var(buffer, input, state);
// 	}
// 	if (var == 1)
// 		*new = ft_create_token(TK_D_QUOTE_7, start, *input - start);
// 	else if (var == 2)
// 		*new = ft_create_token(TK_DOLLAR_8, buffer, ft_strlen_v(buffer));
// 	if (buffer)
// 		free(buffer);
// 	(*input)++;
// 	return (*new);
// }

// static int	ft_dquote_ciclo_var(t_shell_state *state, char **input, char *buffer)
// {
// 	int	var;

// 	var = 1;
// 	while (**input && **input != '"')
// 	{
// 		if (var != 2)
// 			var = ft_check_var(input);
// 		buffer = ft_create_var(buffer, input, state);
// 	}
// 	return (var);
// }

// static t_token	*ft_create_token_dquote(t_token_type type, char *start, int len, int wspace)
// {
// 	t_token	*new;

// 	new = malloc(sizeof(t_token));
// 	if (!new)
// 		return (NULL);
// 	new->type = type;
// 	if (wspace == 1)
// 	{
// 		new->value = malloc(len + 2); // +1 per spazio, +1 per '\0'
// 		new->value = malloc(len + 2); // len caratteri + spazio + '\0'
// 		if (!new->value)
//     		return (free(new), NULL);
// 		ft_memcpy(new->value, start, len);
// 		new->value[len] = ' ';
// 		new->value[len + 1] = '\0';
// 	}
// 	else
// 	{
// 		new->value = ft_strndup(start, len);
// 		if (!new->value)
// 			return (free(new), NULL);
// 	}
// 	new->next = NULL;
// 	return (new);
// }

static t_token	*ft_create_token_dquote(t_token_type type, char *start, int len, int wspace)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->type = type;
	new->value = ft_strndup(start, len);
	new->wspace = wspace;
	new->next = NULL;
	return (new);
}

int	ft_check_token_space(char **input)
{
	int		wspace;
	char	*wspaceptr;
	
	wspace = 0;
	wspaceptr = *input;
	wspaceptr++;
	if (*wspaceptr == ' ')
		wspace = 1;
	return (wspace);
}

// t_token	*ft_dquote(t_shell_state *state, t_token *token, t_token **new, char **input)
// {
// 	char	*start;
// 	char	*buffer;
// 	int		var;

// 	start = *input;
// 	buffer = NULL;
// 	var = 1;
// 	(*input)++;
// 	start = *input;
// 	if (ft_check_dquote(state, token, start) == 1)
// 		return (NULL);
// 	while (**input && **input != '"')
// 	{
// 		if (var != 2)
// 			var = ft_check_var(input);
// 		buffer = ft_create_var(buffer, input, state);
// 	}
// 	if (var == 1)
// 		*new = ft_create_token(TK_D_QUOTE_7, start, *input - start);
// 	else if (var == 2)
// 		*new = ft_create_token(TK_DOLLAR_8, buffer, ft_strlen_v(buffer));
// 	if (buffer)
// 		free(buffer);
// 	(*input)++;
// 	return (*new);
// }

t_token	*ft_dquote(t_shell_state *state, t_token *token, t_token **new, char **input)
{
	char	*start;
	char	*buffer;
	int		var;
	int		wspace;

	var = 1;
	start = *input;
	buffer = NULL;
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
	wspace = ft_check_token_space(input);
	if (var == 1)
		*new = ft_create_token_dquote(TK_D_QUOTE_7, start, *input - start, wspace);
	else if (var == 2)
		*new = ft_create_token_dquote(TK_DOLLAR_8, buffer, ft_strlen_v(buffer), wspace);
	if (buffer)
		free(buffer);
	(*input)++;
	return (*new);
}

t_token	*ft_squote(t_token *token, t_token **new, char **input)
{
	char	*start;
	int		wspace;

	(void)token;
	(*input)++;
	start = *input;
	while (**input && **input != '\'')
		(*input)++;
	if (**input != '\'')
		return (ft_putstr_stderr("Unclosed single quote\n"), NULL);
	wspace = ft_check_token_space(input);
	*new = ft_create_token_dquote(TK_S_QUOTE_6, start, *input - start, wspace);
	(*input)++;
	return (*new);
}

static t_token	*ft_create_token_word(t_token_type type, char *start, int len)
{
	t_token	*new;
	char	*wspaceptr;
	int		i;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->type = type;
	new->value = ft_strndup(start, len);
	new->wspace = 0;
	i = 0;
	wspaceptr = start;
	while (len > 0)
	{
		wspaceptr++;
		len--;
	}
	if (*wspaceptr == ' ')
		new->wspace = 1;
	new->next = NULL;
	return (new);
}

t_token	*ft_word(t_shell_state *state, t_token **new, char **input)
{
	char	*start;
	char	*buffer;
	int		var;
	t_token	*result;

	start = *input;
	buffer = NULL;
	var = 1;
	result = NULL;
	while (**input && **input != ' ' && **input != '|' && **input != '<' && \
			**input != '>' && **input != '\'' && **input != '"')
	{
		if (var != 2)
			var = ft_check_var(input);
		buffer = ft_create_var(buffer, input, state);
	}
	if (var == 1)
		result = ft_create_token_word(TK_WORD_0, start, *input - start);
	else if (var == 2)
		result = ft_create_token(TK_DOLLAR_8, buffer, ft_strlen_v(buffer));
	free(buffer);
	*new = result;
	return (result);
}


t_token	*ft_create_token(t_token_type type, char *start, int len)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->type = type;
	new->value = ft_strndup(start, len);
	new->next = NULL;
	new->wspace = 1;
	return (new);
}