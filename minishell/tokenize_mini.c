/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_mini.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/07/11 17:55:43 by vloddo           ###   ########.fr       */
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
