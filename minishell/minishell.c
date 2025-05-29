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
	const char	*start;
	char quote;

	head = NULL;
	tail = NULL;
	while (*input)
	{
		while (*input == ' ') // ignora spazi
			input++;
		if (*input == '\0') // fine stringa
			break;
		new = NULL;
		if (*input == '|') 
		{
			new = ft_create_token(TK_PIPE_1, input, 1);
			input++;
		}
		else if (*input == '<')
		{
			if (*(input+1) == '<')
			{
				new = ft_create_token(TK_HEREDOC_5, input, 2);
				input += 2;
			} else 
			{
				new = ft_create_token(TK_REDIR_IN_2, input, 1);
				input++;
			}
		}
		else if (*input == '>')
		{
			if (*(input+1) == '>')
			{
				new = ft_create_token(TK_REDIR_APPEND_4, input, 2);
				input += 2;
			} else 
			{
				new = ft_create_token(TK_REDIR_OUT_3, input, 1);
				input++;
			}
		}
		else if (*input == '\'')
		{
			quote = *input++;
			start = input;
			while (*input && *input != quote)
				input++;
			if (*input != quote)
				return (ft_error(token, "Unclosed quote", input), NULL);
			new = ft_create_token(TK_S_QUOTE_6, start, input - start);
			input++; // salta chiusura quote
		}
		else if (*input == '"')
		{
			quote = *input++;
			start = input;
			while (*input && *input != quote)
				input++;
			if (*input != quote)
				return (ft_error(token, "Unclosed quote", input), NULL);
			new = ft_create_token(TK_D_QUOTE_7, start, input - start);
			input++; // salta chiusura quote
		}
		else
		{
			start = input;
			while (*input && *input != ' ' && *input != '|' &&
					*input != '<' && *input != '>' &&
					*input != '\'' && *input != '"')
				input++;
			new = ft_create_token(TK_WORD_0, start, input - start);
		}
		if (!new)
			return (NULL);
		if (!head)
			head = new;
		else
			tail->next = new;
		tail = new;
	}
	return (head);
}

t_token	*ft_create_token(t_token_type type, const char *start, int len)
{
	t_token *new;

	new = malloc(sizeof(t_token));
	if (!new)
		return NULL;

	new->type = type;
	new->value = ft_strndup(start, len);
	new->next = NULL;
	return new;
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*dup;
	size_t	i;

	dup = (char *)malloc(n + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (i < n && s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

void	ft_print_token(t_token *token)
{
	int	i;

	i = 0;
	while (token)
	{
		printf("Token[%d]: Type=%d, Value='%s'\n", i++, token->type, token->value);
		token = token->next;
	}
}

int	main(void)
{
	char	*input;
	t_token *token;
	
	token = NULL;
	while (1)
	{
		input = readline("minishell$ "); // Se l’utente digita qualcosa e preme Invio, readline() ritorna un puntatore a una stringa allocata dinamicamente contenente quel testo (senza il carattere \n).
		if (!input) // Se l’utente preme Ctrl-D a inizio riga, readline() ritorna NULL.
			break; // il programma termina
		if (input && *input) // se input != NULL e non vuota
			add_history(input); // aggiungere una riga di testo alla cronologia dei comandi.
		token = ft_tokenize(token, input); // conta i token 
		ft_print_token(token);
		ft_free_token(token);
		free(input);
	}
	rl_clear_history(); // NON prende parametri.
	return (0);
}