#include "libft.h"

int	print_hex(unsigned int n, int uppercase)
{
	char	*base;
	char	c;
	int		count;

	count = 0;
	if (uppercase)
		base = "0123456789ABCDEF";
	else
		base = "0123456789abcdef";
	if (n >= 16)
		count += print_hex(n / 16, uppercase);
	c = base[n % 16];
	count += write(1, &c, 1);
	return (count);
}
