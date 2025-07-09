/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:29:15 by vloddo            #+#    #+#             */
/*   Updated: 2024/12/11 13:29:17 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	count_words(const char *s, char c)
{
	int	count;

	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s && *s != c)
			s++;
	}
	return (count);
}

static void	ft_free(char **s, int i)
{
	while (i-- > 0)
		free(s[i]);
	free(s);
}

static int	word_len(const char *s, unsigned int start, char end)
{
	int	i;

	i = 0;
	while (s[start] != end && s[start] != 0)
	{
		i++;
		start++;
	}
	return (i);
}

static char	**fill_split(char **dest, const char *s, char c, int words)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	while (i < words)
	{
		while (s[j] == c)
			j++;
		dest[i] = (char *)malloc(sizeof(char) * (word_len(s, j, c) + 1));
		if (!dest[i])
			return (ft_free(dest, i), (NULL));
		k = 0;
		while (s[j] && s[j] != c)
			dest[i][k++] = s[j++];
		dest[i][k] = '\0';
		i++;
	}
	dest[i] = (NULL);
	return (dest);
}

char	**ft_split(char const *s, char c)
{
	char	**dest;
	int		words;

	if (!s)
		return (NULL);
	words = count_words(s, c);
	dest = (char **)malloc(sizeof(char *) * (words + 1));
	if (!dest)
		return (NULL);
	return (fill_split(dest, s, c, words));
}
