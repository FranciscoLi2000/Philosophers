#include "ft_printf.h"

int	print_char(char c)
{
	return (write(1, &c, 1));
}

int	print_string(char *s)
{
	if (!s)
		return (write(1, "(null)", 6));
	ft_putstr_fd(s);
	return (ft_strlen(s));
}

int	print_int(int n)
{
	long	num;
	char	c;
	int		count;

	count = 0;
	num = n;
	if (num < 0)
	{
		count += write(1, "-", 1);
		num = -num;
	}
	if (num >= 10)
		count += print_int(num / 10);
	c = (num % 10) + '0';
	count += write(1, &c, 1);
	return (count);
}

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
