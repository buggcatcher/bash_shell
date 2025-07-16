/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check_mini.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/07/16 17:10:06 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_syntax(t_token *token)
{
	t_token	*tmp;

	if (!token)
		return (1);
	if (ft_is_operator(token))
		return (write(2, "bash: syntax error\n", 20), 1);
	tmp = token;
	while (tmp && tmp->next)
	{
		if (ft_is_operator(tmp) && ft_is_operator(tmp->next))
			return (write(2, "bash: syntax error\n", 20), 1);
		tmp = tmp->next;
	}
	if (ft_is_operator(tmp))
		return (write(2, "bash: syntax error\n", 20), 1);
	return (0);
}

int	ft_is_operator(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == TK_PIPE_1)
		return (1);
	if (token->type == TK_REDIR_IN_2 && token->next == NULL)
		return (1);
	if (token->type == TK_REDIR_OUT_3 && token->next == NULL)
		return (1);
	if (token->type == TK_REDIR_APPEND_4 && token->next == NULL)
		return (1);
	if (token->type == TK_HEREDOC_5 && token->next == NULL)
		return (1);
	return (0);
}
