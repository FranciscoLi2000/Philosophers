#include "libft.h"

void	*ft_calloc(unsigned int nmemb, unsigned int size)
{
	void	*ptr;
	size_t	i;

	// 防止整数溢出：如果 nmemb * size 超过 size_t 范围
	if (size != 0 && nmemb > SIZE_MAX / size)
		return (NULL);
	ptr = malloc(nmemb * size);
	if (!ptr)
		return (NULL);

	/* 推荐调用自己实现的 ft_bzero
	ft_bzero(ptr, nmemb * size);
*/
	// 如果不允许调用 ft_bzero，也可以手动写循环
	i = 0;
	while (i < nmemb * size)
		((unsigned char *)ptr)[i++] = 0;

	return (ptr);
}
