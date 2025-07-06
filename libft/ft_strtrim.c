#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int	start;
	unsigned int	end;
	char		*result;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && ft_strchr(set, s1[start]))
		start++;
	end = (unsigned int)ft_strlen((char *)s1);
	while (end > start && ft_strchr(set, s1[end - 1]))
		end--;
	result = malloc((end - start + 1) * sizeof(char));
	if (!result)
		return (NULL);
	ft_strlcpy(result, &s1[start], end - start + 1);
	return (result);
}
