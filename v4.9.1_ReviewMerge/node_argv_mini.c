/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_argv_mini.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/07/07 20:46:41 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	**ft_build_argv(t_token *start, t_token *end)
// {
// 	char	**argv;
// 	int		j;

// 	j = ft_count_tokens(start, end);
// 	argv = malloc((j + 1) * sizeof(char *));
// 	if (!argv)
// 		return (NULL);
// 	argv = populate_argv(argv, start, end);
// 	return (argv);
// }

// int	ft_count_tokens(t_token *start, t_token *end)
// {
// 	t_token	*tmp;
// 	int		count;

// 	tmp = start;
// 	count = 0;
// 	while (tmp && tmp != end)
// 	{
// 		tmp = skip_redirection_tokens(tmp);
// 		if (!tmp || tmp == end)
// 			break ;
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
// 	while (tmp && tmp != end)
// 	{
// 		tmp = skip_redirection_tokens(tmp);
// 		if (!tmp || tmp == end)
// 			break ;
// 		if (tmp->type == TK_WORD_0 || tmp->type == TK_S_QUOTE_6 || \
// 			tmp->type == TK_D_QUOTE_7 || tmp->type == TK_DOLLAR_8)
// 		{
// 			if (ft_argv(tmp, argv, i) == -1)
// 				return (NULL);
// 			i++;
// 		}
// 		tmp = tmp->next;
// 	}
// 	argv[i] = NULL;
// 	return (argv);
// }

// t_token	*skip_redirection_tokens(t_token *tmp)
// {
// 	if (tmp && tmp->type >= TK_REDIR_IN_2 && tmp->type <= TK_HEREDOC_5)
// 	{
// 		tmp = tmp->next;
// 		if (tmp)
// 			tmp = tmp->next;
// 	}
// 	return (tmp);
// }

// ORIGIN ARGV
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

// NEW
// static char	*ft_strcpy_v(char *s1, const char *s2)
// {
// 	int	i;

// 	i = 0;
// 	while (s2[i] != '\0')
// 	{
// 		s1[i] = s2[i];
// 		i++;
// 	}
// 	s1[i] = '\0';
// 	return (s1);
// }


// int	ft_argv(t_token *tmp, char **argv, int i)
// {
// 	char	*str;
// 	size_t	len;

// 	if (!tmp || !tmp->value)
// 		return (-1);
// 	len = ft_strlen(tmp->value);

// 	if (i > 0 && tmp->wspace == 1)
// 	{
// 		str = malloc(len + 2);
// 		if (!str)
// 		{
// 			while (i-- > 0)
// 				free(argv[i]);
// 			free(argv);
// 			return (-1);
// 		}
// 		ft_strcpy_v(str, tmp->value);
// 		str[len] = ' ';
// 		str[len + 1] = '\0';
// 		argv[i] = str;
// 	}
// 	else
// 	{
// 		argv[i] = ft_strdup(tmp->value);
// 		if (!argv[i])
// 		{
// 			while (i-- > 0)
// 				free(argv[i]);
// 			free(argv);
// 			return (-1);
// 		}
// 	}
// 	return (0);
// }

// _________________________________________________________________________________________________________________-

static int	is_argument_token(t_token_type type)
{
	return (type == TK_WORD_0 || type == TK_S_QUOTE_6
		|| type == TK_D_QUOTE_7 || type == TK_DOLLAR_8);
}

static t_token	*skip_redirection_tokens(t_token *tmp, t_token *end)
{
	if (tmp && tmp != end && tmp->type >= TK_REDIR_IN_2
		&& tmp->type <= TK_HEREDOC_5)
	{
		tmp = tmp->next;
		if (tmp == end || !tmp)
			return (NULL);
		tmp = tmp->next;
	}
	return (tmp);
}

int	ft_count_tokens(t_token *start, t_token *end)
{
	t_token	*cur;
	int		count;

	cur = start;
	count = 0;
	while (cur && cur != end)
	{
		cur = skip_redirection_tokens(cur, end);
		if (!cur || cur == end)
			break ;
		if (is_argument_token(cur->type))
		{
			count++;
			while (cur->wspace == 0 && cur->next != end)
			{
				cur = cur->next;
				if (!is_argument_token(cur->type))
					break ;
			}
		}
		cur = cur->next;
	}
	return (count);
}

static char	**free_argv(char **argv, int i)
{
	while (i-- > 0)
		free(argv[i]);
	free(argv);
	return (NULL);
}

// ORIGIN
// char *ft_strjoin_token_group(t_token *start, t_token *end)
// {
//     char *result;
//     char *ptr;
//     t_token *current;
//     size_t total_len;

// 	total_len = 0;
//     current = start;
//     while (1)
//     {
//         total_len += ft_strlen(current->value);
//         if (current == end)
//             break;
//         current = current->next;
//     }
//     result = malloc(total_len + 1);
//     if (!result)
//         return (NULL);
//     ptr = result;
//     current = start;
//     while (1)
//     {
//         ft_strcpy(ptr, current->value);
//         ptr += ft_strlen(current->value);
//         if (current == end)
//             break;
//         current = current->next;
//     }
//     *ptr = '\0';
//     return (result);
// }

// Funzione strjoin divisa in due funzioni sotto le 25 righe
static void		ft_concatenate_tokens(t_token *start, t_token *end, char *buffer)
{
	t_token	*current;
	char	*ptr;

	ptr = buffer;
	current = start;
	while (1)
	{
		ft_strcpy(ptr, current->value);
		ptr += ft_strlen(current->value);
		if (current == end)
			break ;
		current = current->next;
	}
	*ptr = '\0';
}

static char	*ft_strjoin_token_group(t_token *start, t_token *end)
{
	char	*result;
	size_t	total_len;
	t_token *current;

	total_len = 0;
	current = start;
    while (1)
	{
		total_len += ft_strlen(current->value);
		if (current == end)
			break;
		current = current->next;
	}
	result = malloc(total_len + 1);
	if (!result)
		return (NULL);
	ft_concatenate_tokens(start, end, result);
	return (result);
}

//ORIGIN
// char	**populate_argv(char **argv, t_token *start, t_token *end)
// {
// 	t_token	*cur;
// 	t_token	*group_start;
// 	t_token	*group_end;
// 	int		i;

// 	cur = start;
// 	i = 0;
// 	while (cur && cur != end)
// 	{
// 		cur = skip_redirection_tokens(cur, end);
// 		if (!cur || cur == end)
// 			break ;
// 		if (is_argument_token(cur->type))
// 		{
// 			group_start = cur;
// 			group_end = cur;
// 			while (cur->wspace == 0 && cur->next != end)
// 			{
// 				cur = cur->next;
// 				if (!is_argument_token(cur->type))
// 					break ;
// 				group_end = cur;
// 			}
// 			argv[i] = ft_strjoin_token_group(group_start, group_end);
// 			if (!argv[i])
// 				return (free_argv(argv, i));
// 			i++;
// 		}
// 		cur = cur->next;
// 	}
// 	argv[i] = NULL;
// 	return (argv);
// }

// populate argv divisa in 2 funzioni sotto le 25 righe
static t_token	*process_token_group(t_token *cur, t_token *end, char **result_str)
{
	t_token *group_start;
	t_token *group_end;

	group_start = cur;
	group_end = cur;
	while (cur->wspace == 0 && cur->next != NULL && cur->next != end)
	{
		cur = cur->next;
		if (!is_argument_token(cur->type))
			break;
		group_end = cur;
	}
	*result_str = ft_strjoin_token_group(group_start, group_end);
	return (cur);
}

static int	process_argument_token(t_token **cur, t_token *end, char **argv, int *i)
{
	char *group_str;
	t_token *processed_cur;

	group_str = NULL;
	processed_cur = process_token_group(*cur, end, &group_str);
	if (!group_str)
		return (0);
	argv[(*i)++] = group_str;
	*cur = processed_cur;
	return (1);
}

char	**populate_argv(char **argv, t_token *start, t_token *end)
{
	t_token *cur;
	int i;

	cur = start;
	i = 0;
	while (cur && cur != end)
	{
		cur = skip_redirection_tokens(cur, end);
		if (!cur || cur == end)
			break;
		if (is_argument_token(cur->type))
		{
			if (!process_argument_token(&cur, end, argv, &i))
				return (free_argv(argv, i));
		}
		if (cur->next)
			cur = cur->next;
		else
			break;
	}
	argv[i] = NULL;
	return (argv);
}

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
