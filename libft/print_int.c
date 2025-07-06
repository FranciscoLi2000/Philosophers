#include "libft.h"

static int	print_int_recursive(long nb)
{
	char	c;
	int		count;

	count = 0;
	if (nb >= 10)
		count += print_int_recursive(nb / 10);
	c = (nb % 10) + '0';
	count += write(1, &c, 1);
	return (count);
}

int	print_int(int n)
{
	long	nb;
	int		count;

	nb = n;
	count = 0;
	if (nb < 0)
	{
		count += write(1, "-", 1);
		nb = -nb;
	}
	count += print_int_recursive(nb);
	return (count);
}
