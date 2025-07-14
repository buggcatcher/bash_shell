/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_argv_mini.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/07/14 21:27:13 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_argument_token(t_token_type type)
{
	return (type == TK_WORD_0 || type == TK_S_QUOTE_6
		|| type == TK_D_QUOTE_7 || type == TK_DOLLAR_8);
}

static t_token	*skip_redirection_tokens(t_token *tmp, t_token *end)
{
	while (tmp && tmp != end && (tmp->type >= TK_REDIR_IN_2
		&& tmp->type <= TK_HEREDOC_5))
	{
		tmp = tmp->next;
		if (tmp == end || !tmp)
			return (NULL);
		tmp = tmp->next;
	}
	//printf("SKIP REDIRECTION %s\n", tmp->value);
	return (tmp);
}

/*int	ft_count_tokens(t_token *start, t_token *end)
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
}*/

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
			while (cur->wspace == 0 && cur->next != NULL && cur->next != end)
			{
				if (!is_argument_token(cur->type))
					break ;
				cur = cur->next;
			}
		}
		if (is_argument_token(cur->type))
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

/*static void		ft_concatenate_tokens(t_token *start, t_token *end, char *buffer, int first_arg)
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
	if (!first_arg && end->wspace == 1)
	{
		*ptr = ' ';
		ptr++;
	}
	*ptr = '\0';
}*/
static void		ft_concatenate_tokens(t_token *start, t_token *end, char *buffer, int first_arg)
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
	if (!first_arg && end->wspace == 1 && current != end)
	{
		*ptr = ' ';
		ptr++;
	}
	*ptr = '\0';
}

/*static char	*ft_strjoin_token_group(t_token *start, t_token *end, int first_arg)
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
	if (!first_arg && end->wspace == 1)
		total_len++;
	result = malloc(total_len + 1);
	if (!result)
		return (NULL);
	ft_concatenate_tokens(start, end, result, first_arg);
	return (result);
}*/

static char	*ft_strjoin_token_group(t_token *start, t_token *end, int first_arg)
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
	if (!first_arg && end->wspace == 1 && current != end)
		total_len++;
	result = malloc(total_len + 1);
	if (!result)
		return (NULL);
	ft_concatenate_tokens(start, end, result, first_arg);
	return (result);
}

/*static t_token	*process_token_group(t_token *cur, t_token *end, char **result_str, int first_arg)
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
	*result_str = ft_strjoin_token_group(group_start, group_end, first_arg);
	return (cur);
}*/

static t_token	*process_token_group(t_token *cur, t_token *end, char **result_str, int first_arg)
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
	*result_str = ft_strjoin_token_group(group_start, group_end, first_arg);
	return (cur);
}

static int	process_argument_token(t_token **cur, t_token *end, char **argv, int *i)
{
	char	*group_str;
	t_token	*processed_cur;
	int		first_arg = (*i == 0);

	group_str = NULL;
	processed_cur = process_token_group(*cur, end, &group_str, first_arg);
	if (!group_str)
		return (0);
	argv[(*i)++] = group_str;
	*cur = processed_cur;
	return (1);
}

/*char	**populate_argv(char **argv, t_token *start, t_token *end)
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
}*/

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
			cur = skip_redirection_tokens(cur, end);
			if (!cur || cur == end)
				break;
		}
		if (cur->next)
			cur = cur->next;
		else
			break;
	}
	printf("[%d]\n", i);
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
