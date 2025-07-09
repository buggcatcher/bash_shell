/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vloddo <vloddo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 12:50:13 by vloddo            #+#    #+#             */
/*   Updated: 2025/06/23 19:17:56 by vloddo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	count_words( char *s, char c)
{
	size_t	i;
	size_t	words;

	i = 0;
	words = 0;
	while (s && s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
			words++;
		while (s[i] && s[i] != c)
			i++;
	}
	return (words);
}

static char	*dup_word(char *s, char c)
{
	size_t	len;
	char	*word;
	size_t	i;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	word = safe_alloc(len + 1, sizeof(char), "dup_word");
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[len] = '\0';
	return (word);
}

static int	fill_split(char **res, char *s, char c)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
		{
			res[j] = dup_word(&s[i], c);
			if (!res[j])
			{
				free_split_all(res);
				return (0);
			}
			j++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	res[j] = NULL;
	return (1);
}

char	**ft_split(char *s, char c)
{
	size_t	words;
	char	**res;

	if (!s)
		return (NULL);
	words = count_words(s, c);
	res = safe_alloc(words + 1, sizeof(char *), "ft_split");
	if (!res)
		return (NULL);
	if (!fill_split(res, s, c))
		return (NULL);
	return (res);
}
