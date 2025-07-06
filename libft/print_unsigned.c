#include "libft.h"

int	print_unsigned(unsigned int n)
{
	char	c;
	int		count;

	count = 0;
	if (n >= 10)
		count += print_unsigned(n / 10);
	c = (n % 10) + '0';
	count += write(1, &c, 1);
	return (count);
}
