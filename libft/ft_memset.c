#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char		*ptr;
	unsigned char		uc;

	ptr = s;
	uc = (unsigned char)c;
	i = 0;
	while (i < n)
	{
		ptr[i] = uc;
		i++;
	}
	return (s);
}
