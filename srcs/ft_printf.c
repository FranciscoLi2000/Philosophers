#include "ft_printf.h"

int	print_ptr(unsigned long long ptr)
{
	int	count;

	if (!ptr)
		return (write(1, "(nil)", 5));
	count = 2;
	write(1, "0x", 2);
	while (ptr >= 16)
	{
		print_ptr(ptr / 16);
		count++;
	}
	return (count);
}

int	handle_format(char specifier, va_list args)
{
	int	count;

	count = 0;
	if (specifier == 'c')
		count += print_char();
	else if (specifier == 's')
		count += print_string();
	else if (specifier == 'd' || specifier == 'i')
		count += print_int();
	else if (specifier == 'u')
		count += print_unsigned();
	else if (specifier == 'x')
		count += print_hex();
	else if (specifier == 'X')
		count += print_hex();
	else if (specifier == '%')
		count += print_char();
	return (count);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;

	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			if (ft_isalpha(*format))
				handle_format(*format, args);
		}
		else
			ft_putstr_fd(format, 1);
	}
	va_end(args);
	return (count);
}
