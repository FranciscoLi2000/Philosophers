#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*dst;
	int		i;
	int		len;

	if (!s)
		return (NULL);
	len = 0;
	while (s[len])
		len++;
	dst = malloc((len + 1) * sizeof(char));
	if (!dst)
		return (NULL);
	i = 0;
	while (s[i])
	{
		dst[i] = s[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
