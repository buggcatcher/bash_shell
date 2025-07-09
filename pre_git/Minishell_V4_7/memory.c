/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/23 18:01:27 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*str;

	i = 0;
	str = s;
	while (i < n)
		str[i++] = '\0';
}

void	*ft_calloc(size_t n_elem, size_t size)
{
	void	*array;

	array = malloc(n_elem * size);
	if (!array)
		return (NULL);
	ft_bzero(array, n_elem * size);
	return (array);
}

void	*safe_alloc(size_t n_elem, size_t bytes, char *description)
{
	void	*ptr;

	ptr = ft_calloc(n_elem, bytes);
	if (!ptr)
	{
		printf("Error: memory allocation for [%s] failed\n", description);
		return (NULL);
	}
	return (ptr);
}
