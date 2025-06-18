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

/*
char	**ft_build_argv(t_token *start, t_token *end)
{
	t_token *tmp;
	char **argv;
	int i;
	int	j;

	tmp = start;
	j = 0;
	while (tmp != end && tmp) // Conta i token di tipo WORD (compresi quote e dollar)
	{
		if (tmp->type == TK_WORD_0 || tmp->type == TK_S_QUOTE_6 || tmp->type == TK_D_QUOTE_7 || tmp->type == TK_DOLLAR_8)
			j++;
		tmp = tmp->next;
	}
	argv = malloc((j + 1) * sizeof(char *)); // Alloca l'array di stringhe
	if (!argv)
		return (NULL);
	i = 0; // Popola l'array
	tmp = start;
	while (tmp != end && tmp)
	{
		if (tmp->type == TK_WORD_0 || tmp->type == TK_S_QUOTE_6 || tmp->type == TK_D_QUOTE_7 || tmp->type == TK_DOLLAR_8)
		{
			argv[i] = ft_strdup(tmp->value);
			if (!argv[i])
			{
				while (i-- > 0) // Libera memoria in caso di errore
					free(argv[i]);
				free(argv);
				return (NULL);
			}
			i++;
		}
		tmp = tmp->next;
	}
	argv[i] = NULL;
	return (argv);
} */

char	**ft_build_argv(t_token *start, t_token *end)
{
	char	**argv;
	int		j;

	j = ft_count_tokens(start, end);
	argv = malloc((j + 1) * sizeof(char *));
	if (!argv)
		return (NULL);
	argv = populate_argv(argv, start, end);
	return (argv);
}


// int	ft_count_tokens(t_token *start, t_token *end)
// {
// 	t_token	*tmp;
// 	int		count;

// 	tmp = start;
// 	count = 0;
// 	while (tmp && tmp != end)
// 	{
// 		if (tmp->type == TK_WORD_0 || tmp->type == TK_S_QUOTE_6
// 			|| tmp->type == TK_D_QUOTE_7 || tmp->type == TK_DOLLAR_8)
// 			count++;
// 		tmp = tmp->next;
// 	}
// 	return (count);
// }

// char	**populate_argv(char **argv, t_token *start, t_token *end)
// {
// 	t_token	*tmp;
// 	int		i;

// 	i = 0;
// 	tmp = start;
// 	while (tmp && tmp != end )
// 	{
// 		if (tmp->type == TK_WORD_0 || tmp->type == TK_S_QUOTE_6 || 
// 			tmp->type == TK_D_QUOTE_7 || tmp->type == TK_DOLLAR_8)
// 		{
// 			argv[i] = ft_strdup(tmp->value);
// 			if (!argv[i])
// 			{
// 				while (i-- > 0)
// 					free(argv[i]);
// 				free(argv);
// 				return (NULL);
// 			}
// 			i++;
// 		}
// 		tmp = tmp->next;
// 	}
// 	argv[i] = NULL;
// 	return (argv);
// }

int	ft_count_tokens(t_token *start, t_token *end)
{
	t_token	*tmp;
	int		count;

	tmp = start;
	count = 0;
	while (tmp && tmp != end)
	{
		tmp = skip_redirection_tokens(tmp);
		if (!tmp || tmp == end)
			break;
		if (tmp->type == TK_WORD_0 || tmp->type == TK_S_QUOTE_6
			|| tmp->type == TK_D_QUOTE_7 || tmp->type == TK_DOLLAR_8)
			count++;
		tmp = tmp->next;
	}
	return (count);
}

char	**populate_argv(char **argv, t_token *start, t_token *end)
{
	t_token	*tmp;
	int		i;

	i = 0;
	tmp = start;
	while (tmp && tmp != end )
	{
		tmp = skip_redirection_tokens(tmp);
		if (!tmp || tmp == end)
			break;
		if (tmp->type == TK_WORD_0 || tmp->type == TK_S_QUOTE_6 || 
			tmp->type == TK_D_QUOTE_7 || tmp->type == TK_DOLLAR_8)
		{
			if (ft_argv(tmp, argv, i) == -1)
				return (NULL);
			i++;
		}
		tmp = tmp->next;
	}
	argv[i] = NULL;
	return (argv);
}

t_token	*skip_redirection_tokens(t_token *tmp)
{
	if (tmp && tmp->type >= TK_REDIR_IN_2 && tmp->type <= TK_HEREDOC_5)
	{
		tmp = tmp->next; // salto filename
		if (tmp)
			tmp = tmp->next;
	}
	return (tmp);
}

int	ft_argv(t_token *tmp, char **argv, int i)
{
	argv[i] = ft_strdup(tmp->value);
	if (!argv[i])
	{
		while (i-- > 0)
			free(argv[i]);
		free(argv);
		return (-1);
	}
	return (0);
}

