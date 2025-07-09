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

size_t	ft_strlen_v(char *a)
{
	size_t	i;

	i = 0;
	while (a[i] != '\0')
	{
		i++;
	}
	return (i);
}

char	*ft_strndup(char *s, size_t n)
{
	char	*dup;
	size_t	i;

	if (!s)
		return (NULL);
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

char	*ft_strdup(char *s)
{
	char	*c;
	size_t	i;

	i = ft_strlen_v(s);
	c = (char *)malloc(i + 1);
	if (c != NULL)
	{
		i = 0;
		while (s[i] != '\0')
		{
			c[i] = s[i];
			i++;
		}
		c[i] = '\0';
		return ((char *)c);
	}
	return (NULL);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*final;
	int		i;
	int		j;

	i = ft_strlen_v(s1);
	j = ft_strlen_v(s2);
	final = (char *)malloc(sizeof(char) * (i + j + 1));
	if (!final)
		return (NULL);
	final[i + j] = '\0';
	while (j > 0)
	{
		j--;
		final[i + j] = s2[j];
	}
	while (i > 0)
	{
		i--;
		final[i] = s1[i];
	}
	return (final);
}

char	*ft_append_char(char *s, char c)
{
	int		len;
	char	*new;
	int		i;

	if (!s)
	{
		new = malloc(2);
		new[0] = c;
		new[1] = '\0';
		return (new);
	}
	len = ft_strlen_v(s);
	new = (char *)malloc(sizeof(char) * (len + 2));
	if (!new)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new[i] = s[i];
		i++;
	}
	new[i++] = c;
	new[i] = '\0';
	free(s);
	return (new);
}
