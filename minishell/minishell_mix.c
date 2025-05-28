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
	{ // if s[j] == " || '
		// quando incontra quotes imposta start
		// i++
		// quando incontra quotes imosta end
		// poi se non trova alte quotes errore di sintassi

		
		s[j] == c
		while (s[j]  == c)
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

char	**ft_get_path(char **envp)
{
	int		i;
	char	**temp;

	i = 0;
	while (envp[i])
	{
		if ((ft_strncmp(envp[i], "PATH=", 5) == 0))
		{
			temp = ft_split((envp[i] + 5), ':');
			return (temp);
		}
		i++;
	}
	ft_error("Path", 4);
	return (NULL);
}

char	*check_args(char **paths, char *argcmd)
{
	char	*directory;
	char	*cmd;
	int		i;

	i = 0;
	while (paths[i])
	{
		directory = ft_strjoin(paths[i], "/");
		cmd = ft_strjoin(directory, argcmd);
		free(directory);
		if (access(cmd, X_OK) == 0)
		{
			return (cmd);
		}
		free(cmd);
		i++;
	}
	return (NULL);
}


int main(int argc, char **argv, char **envp)
{
	char	*cmd;
    
	cmd = ft_split(argv[1], ' ');
	if (cmd)
		write(2, "zsh: Command not found\n", 24);
}
