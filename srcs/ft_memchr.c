#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	const unsigned char	*ptr;
	unsigned char		uc;

	ptr = s;
	uc = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		if (ptr[i] == uc)
			return ((void *)(ptr + i));
		i++;
	}
	return (NULL);
}
