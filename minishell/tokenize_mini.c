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

t_token	*ft_tokenize(t_token *token, char *input)
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
		if (!ft_get_token(token, &input, &new))
			return (NULL);
		if (!head)
			head = new;
		else
			tail->next = new;
		tail = new;
	}
	return (head);
}

t_token	*ft_get_token(t_token *token, char **input, t_token **new)
{
	*new = NULL;
	if (**input == '|')
		*new = ft_pipe(new, input);
	else if (**input == '<')
		*new = ft_redher(new, input);
	else if (**input == '>')
		*new = ft_redred(new, input);
	else if (**input == '\'')
		*new = ft_squote(token, new, input);
	else if (**input == '"')
		*new = ft_dquote(token, new, input);
	else
		*new = ft_word(new, input);
	return (*new);
}

t_token *ft_word(t_token **new, char **input)
{
	char	*start;
	char		*buffer;
	int			var;

	start = *input;
	buffer = NULL;
	var = 1;
	while (**input && **input != ' ' && **input != '|' && **input != '<' && **input != '>' && 
			**input != '\'' && **input != '"')  
	{
		if (var != 2)
			var = ft_check_var(input);			
		buffer = ft_create_var(buffer, input);
	}
	if (var == 1)
		*new = ft_create_token(TK_WORD_0, start, *input - start);
	else if (var == 2)
		*new = ft_create_token(TK_DOLLAR_8, buffer, ft_strlen(buffer));
	free(buffer);
	return (*new);
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
	return (new);
}

