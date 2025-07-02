/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_mini.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/07/02 17:40:40 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*ft_tokenize(t_shell_state *state, t_token *token, char *input)
{
	t_token	*head;
	t_token	*tail;
	t_token	*new;

	head = NULL;
	tail = NULL;
	while (*input)
	{
		while (*input == ' ')
			input++;
		if (*input == '\0')
			break ;
		if (!ft_get_token(state, token, &input, &new))
		{
			ft_free_token(head);
			return (NULL);
		}
		if (!head)
			head = new;
		else
			tail->next = new;
		tail = new;
	}
	return (head);
}

t_token	*ft_get_token(t_shell_state *s, t_token *t, char **i, t_token **n)
{
	*n = NULL;
	if (**i == '|')
		*n = ft_pipe(n, i);
	else if (**i == '<')
		*n = ft_redher(n, i);
	else if (**i == '>')
		*n = ft_redred(n, i);
	else if (**i == '\'')
		*n = ft_squote(t, n, i);
	else if (**i == '"')
		*n = ft_dquote(s, t, n, i);
	else
		*n = ft_word(s, n, i);
	return (*n);
}

// t_token	*ft_word(t_shell_state *state, t_token **new, char **input)
// {
// 	char	*start;
// 	char	*buffer;
// 	int		var;
// 	t_token	*result;

// 	start = *input;
// 	buffer = NULL;
// 	var = 1;
// 	result = NULL;
// 	while (**input && **input != ' ' && **input != '|' && **input != '<' && \
// 			**input != '>' && **input != '\'' && **input != '"')
// 	{
// 		if (var != 2)
// 			var = ft_check_var(input);
// 		buffer = ft_create_var(buffer, input, state);
// 	}
// 	if (var == 1)
// 		result = ft_create_token(TK_WORD_0, start, *input - start);
// 	else if (var == 2)
// 		result = ft_create_token(TK_DOLLAR_8, buffer, ft_strlen_v(buffer));
// 	free(buffer);
// 	*new = result;
// 	return (result);
// }

// t_token	*ft_create_token(t_token_type type, char *start, int len)
// {
// 	t_token	*new;

// 	new = malloc(sizeof(t_token));
// 	if (!new)
// 		return (NULL);
// 	new->type = type;
// 	new->value = ft_strndup(start, len);
// 	new->next = NULL;
// 	return (new);
// }


