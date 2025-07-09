#include "minishell.h"

/**
 * string character restituisce un puntatore alla prima occorrenza del carattere 'c' nella stringa 's'.
 * Se 'c' è '\0', ritorna il puntatore al terminatore di fine stringa.
 * Se non trovato, ritorna NULL.
 */
char	*ft_strchr( char *s, int c)
{
	char	ch;

	ch = (char)c;
	while (*s)
	{
		if (*s == ch)
			return ((char *)s); 
		s++;
	}
	if (ch == '\0')
		return ((char *)s);
	return (NULL);
}


int switch_fd(int from, int to)
{
	if (from != to)
	{
		if (dup2(from, to) == -1)
		{
			perror("dup2");
			return 1;
		}
	}
	return 0;
}


int save_fd(int fd)
{
	int clone = dup(fd);
	if (clone == -1)
	{
		perror("dup");
		exit(EXIT_FAILURE); // valutare se invece restituire errore e propagare
	}
	return clone;
}


int open_outfile( char *filename)
{
	int	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("error opening outfile");
		return -1;
	}
	return fd;
}


int save_stdout(void)
{
	int clone = dup(1);
	if (clone == -1)
	{
		perror("dup stdout");
		exit(EXIT_FAILURE);
	}
	return clone;
}

// --- 

// --- memoria ---


void ft_bzero(void *s, size_t n)
{
	size_t i = 0;
	char *str = s;
	while (i < n)
		str[i++] = '\0';
}


void *ft_calloc(size_t n_elem, size_t size)
{
	void *array = malloc(n_elem * size);
	if (!array)
		return NULL;
	ft_bzero(array, n_elem * size);
	return array;
}


void *safe_alloc(size_t n_elem, size_t bytes,  char *description)
{
	void *ptr = ft_calloc(n_elem, bytes);
	if (!ptr)
	{
		printf("Error: memory allocation for [%s] failed\n", description);
		return NULL;
	}
	return ptr;
}


/// --- libfx ---


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
	return words;
}


static char	*dup_word( char *s, char c)
{
	size_t len;
	char *word;

	len = 0;
	while (s[len] && s[len] != c)
		len++;

	word = safe_alloc(len + 1, sizeof(char), "dup_word");
	if (!word)
		return NULL;

	for (size_t i = 0; i < len; i++)
		word[i] = s[i];
	word[len] = '\0';
	return word;
}


char	**ft_split(char  *s, char c)
{
	if (!s)
		return NULL;

	size_t words = count_words(s, c);
	char **res = safe_alloc(words + 1, sizeof(char *), "ft_split");
	if (!res)
		return NULL;

	size_t i = 0, j = 0;
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
				return NULL;
			}
			j++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	res[j] = (NULL);
	return (res);
}


char	*ft_strjoin_m( char *s1,  char *s2)
{
	if (!s1 || !s2)
		return NULL;

	size_t len1 = ft_strlen(s1);
	size_t len2 = ft_strlen(s2);
	char *joined = safe_alloc(len1 + len2 + 1, sizeof(char), "ft_strjoin");
	if (!joined)
		return NULL;

	size_t i = 0;
	while (*s1)
		joined[i++] = *s1++;
	while (*s2)
		joined[i++] = *s2++;
	joined[i] = '\0';
	return joined;
}


size_t	ft_strlen( char *s)
{
	size_t i;

	i = 0;
	while (s && s[i])
		i++;
	return (i);
}


char	*ft_strdup_m(char *s)
{
	size_t len;
	char *dup;
	size_t i;

	len = ft_strlen(s);
	dup = safe_alloc(len + 1, sizeof(char), "ft_strdup");
	i = 0;
	if (!dup)
		return (NULL);
	while (i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[len] = '\0';
	return (dup);
}

void	free_array(char **arr)
{
	int i = 0;

	i = 0;
	if (!arr) 
		return;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int	env_size(t_env *env)
{
	int count;

	count = 0;
	while (env)
	{
		if (env->exported)
			count++;
		env = env->next;
	}
	return (count);
}
