#include "libft.h"

int	ft_strncmp(char *dest, const char *src, unsigned int nb)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < n - 1 && dest[i] != '\0' && src[i] != '\0' && dest[i] == src[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
