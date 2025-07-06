#include "libft.h"

static void	*free_split(char **tab, size_t filled)
{
	size_t	i;

	i = 0;
	while (i < filled)
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

static size_t	count_words(char const *s, char c)
{
	size_t	i;
	size_t	count;
	int		in_word;

	if (!s)
		return (0);
	i = 0;
	count = 0;
	in_word = 0;
	while (s[i])
	{
		if (s[i] == c)
			in_word = 0;
		else if (in_word == 0)
		{
			in_word = 1;
			count++;
		}
		i++;
	}
	return (count);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	start;
	size_t	word;
	size_t	total;
	char	**res;

	if (!s)
		return (NULL);
	total = count_words(s, c);
	res = malloc((total + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	i = 0;
	word = 0;
	while (word < total)
	{
		while (s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		res[word] = ft_substr(s, start, i - start);
		if (!res[word])
			return (free_split(res, word));
		word++;
	}
	res[word] = NULL;
	return (res);
}
