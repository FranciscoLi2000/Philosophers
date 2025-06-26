#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined;
	int		s1_len;
	int		s2_len;
	int		i;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	joined = malloc((s1_len + s2_len + 1) * sizeof(char));
	if (!joined)
		return (NULL);
	i = 0;
	while (i < s1_len + s2_len)
	{
		if (i < s1_len)
			joined[i] = s1[i];
		else
			joined[i] = s2[i - s1_len];
		i++;
	}
	joined[i] = '\0';
	return (joined);
}
