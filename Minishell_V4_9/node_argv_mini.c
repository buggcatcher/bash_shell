/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_argv_mini.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/07/02 17:57:05 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			break ;
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
	while (tmp && tmp != end)
	{
		tmp = skip_redirection_tokens(tmp);
		if (!tmp || tmp == end)
			break ;
		if (tmp->type == TK_WORD_0 || tmp->type == TK_S_QUOTE_6 || \
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
		tmp = tmp->next;
		if (tmp)
			tmp = tmp->next;
	}
	return (tmp);
}

// int	ft_argv(t_token *tmp, char **argv, int i)
// {
// 	argv[i] = ft_strdup(tmp->value);
// 	if (!argv[i])
// 	{
// 		while (i-- > 0)
// 			free(argv[i]);
// 		free(argv);
// 		return (-1);
// 	}
// 	return (0);
// }

static char	*ft_strcpy(char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s2[i] != '\0')
	{
		s1[i] = s2[i];
		i++;
	}
	s1[i] = '\0';
	return (s1);
}

int	ft_argv(t_token *tmp, char **argv, int i)
{
	char	*str;
	size_t	len;

	if (!tmp || !tmp->value)
		return (-1);
	len = ft_strlen(tmp->value);

	if (i > 0 && tmp->wspace == 1)
	{
		str = malloc(len + 2);
		if (!str)
		{
			while (i-- > 0)
				free(argv[i]);
			free(argv);
			return (-1);
		}
		ft_strcpy(str, tmp->value);
		str[len] = ' ';
		str[len + 1] = '\0';
		argv[i] = str;
	}
	else
	{
		argv[i] = ft_strdup(tmp->value);
		if (!argv[i])
		{
			while (i-- > 0)
				free(argv[i]);
			free(argv);
			return (-1);
		}
	}
	return (0);
}
