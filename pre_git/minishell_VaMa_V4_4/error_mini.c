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

int	ft_error(t_token *token, char *msg)
{
	ft_putstr(msg);
	ft_free_token(token);
	return (1);
}

void	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		write(2, &str[i], 1);
		i++;
	}
	write(2, "\n", 1);
}
