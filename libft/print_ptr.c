#include "libft.h"

int	print_ptr(unsigned long long ptr)
{
	char	*base;
	int		count;

	base = "0123456789abcdef";
	count = 0;
	if (!ptr)
		return (write(1, "(nil)", 5));
	count += write(1, "0x", 2);
	if (ptr >= 16)
		count += print_ptr(ptr / 16);
	count += write(1, &base[ptr % 16], 1);
	return (count);
}
