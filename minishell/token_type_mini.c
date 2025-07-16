/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type_mini.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/07/16 19:31:55 by vloddo           ###   ########.fr       */
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

static t_token	*ft_create_token_quote(t_token_type t, char *s, int l, int ws)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->type = t;
	new->value = ft_strndup(s, l);
	new->wspace = ws;
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

static t_token	*ft_create_token_dollar(t_token_type t, char *b, int l, char *s)
{
	t_token	*new;
	char	*wspaceptr;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->type = t;
	new->value = ft_strndup(b, l);
	new->wspace = 0;
	wspaceptr = s;
	while (*wspaceptr && \
			((*wspaceptr >= '0' && *wspaceptr <= '9') || \
			(*wspaceptr >= 'A' && *wspaceptr <= 'Z') || \
			(*wspaceptr >= 'a' && *wspaceptr <= 'z') || \
			*wspaceptr == '_' || *wspaceptr == '$'))
		wspaceptr++;
	if (*wspaceptr == '"' )
		wspaceptr++;
	if (*wspaceptr == ' ')
		new->wspace = 1;
	new->next = NULL;
	return (new);
}

static char	*ft_process_dquote_content(t_shell_state *s, char **i, int *var)
{
	char	*buffer;

	buffer = NULL;
	while (**i && **i != '"')
	{
		if (*var != 2)
			*var = ft_check_var(i);
		buffer = ft_create_var(buffer, i, s);
	}
	return (buffer);
}

t_token	*ft_dquote(t_shell_state *s, t_token *t, t_token **n, char **i)
{
	char	*start;
	char	*buffer;
	int		var;
	int		wspace;

	var = 1;
	start = *i;
	buffer = NULL;
	(*i)++;
	start = *i;
	if (ft_check_dquote(s, t, start) == 1)
		return (NULL);
	buffer = ft_process_dquote_content(s, i, &var);
	wspace = ft_check_token_space(i);
	if (var == 1)
		*n = ft_create_token_quote(TK_D_QUOTE_7, start, *i - start, wspace);
	else if (var == 2)
		*n = ft_create_token_dollar(TK_DOLLAR_8, buffer, ft_strlen_v(buffer), start);
	if (buffer)
		free(buffer);
	(*i)++;
	return (*n);
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
	*new = ft_create_token_quote(TK_S_QUOTE_6, start, *input - start, wspace);
	(*input)++;
	return (*new);
}

static t_token	*ft_create_token_word(t_token_type type, char *start, int len)
{
	t_token	*new;
	char	*wspaceptr;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->type = type;
	new->value = ft_strndup(start, len);
	new->wspace = 0;
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
		result = ft_create_token_dollar(TK_DOLLAR_8, buffer, ft_strlen_v(buffer), start);
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
	new->wspace = 0;
	return (new);
}
