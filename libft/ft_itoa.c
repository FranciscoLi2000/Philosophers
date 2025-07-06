#include "libft.h"

static size_t	num_len(int n)
{
	size_t	len;
	long	num;

	num = n;
	len = (num <= 0);
	if (num < 0)
		num = -num;
	while (num > 0)
	{
		num /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	size_t	len;
	long	num;
	char	*str;

	len = num_len(n);
	num = n;
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (num == 0)
		str[0] = '0';
	if (num < 0)
	{
		str[0] = '-';
		num = -num;
	}
	while (num > 0)
	{
		str[--len] = (num % 10) + '0';
		num /= 10;
	}
	return (str);
}
