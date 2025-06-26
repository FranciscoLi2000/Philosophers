#include "libft.h"

static int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static int	count_words(char *str)
{
	int	i;
	int	count;
	int	in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (str[i])
	{
		if (ft_isspace(str[i]))
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

char	**ft_split(char *str)
{
	char	**result;
	int		i;
	int		start;
	int		end;
	int		word;

	i = 0;
	word = 0;
	if (!str)
		return (NULL);
	result = (char **)malloc((count_words(str) + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	while (str[i])
	{
		while (str[i] && ft_isspace(str[i]))
			i++;
		start = i;
		while (str[i] && !ft_isspace(str[i]))
			i++;
		end = i;
		if (end > start)
			result[word++] = ft_substr(str, start, end - start);
	}
	result[word] = NULL;
	return (result);
}
/*
#include <stdio.h>

int	main(void)
{
	char	**palabras = ft_split("  hola\tmundo\n42  libft ");
	int		i = 0;

	while (palabras[i])
	{
		printf("Palabra %d: %s\n", i, palabras[i]);
		free(palabras[i]);
		i++;
	}
	free(palabras);
	return (0);
}*/
