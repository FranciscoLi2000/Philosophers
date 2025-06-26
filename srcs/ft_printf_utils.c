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
	int	count;

	num = n;
	count = 0;
	if (num < 0)
	{
		write(1, "-", 1);
		count++;
		num = -num;
	}
	while (num >= 10)
	{
		print_int(num / 10);
		count++;
	}
	ft_putchar_fd(num / 10 + '0', 1);
	count++;
	return (count);
}

int	print_unsigned(unsigned int n)
{
	long	num;

	num = n;
	while (num >= 10)
	{
	}
}

int	print_hex(unsigned int n, int uppercase)
{
}
