#include "ft_printf.h"

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

int	handle_format(char specifier, va_list args)
{
	int	count;

	count = 0;
	if (specifier == 'c')
		count += print_char(va_arg(args, int));
	else if (specifier == 's')
		count += print_string(va_arg(args, char *));
	else if (specifier == 'd' || specifier == 'i')
		count += print_int(va_arg(args, int));
	else if (specifier == 'u')
		count += print_unsigned(va_arg(args, unsigned int));
	else if (specifier == 'x')
		count += print_hex(va_arg(args, unsigned int), 0);
	else if (specifier == 'X')
		count += print_hex(va_arg(args, unsigned int), 1);
	else if (specifier == '%')
		count += print_char('%');
	return (count);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		count;

	count = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%' && *(format + 1))
		{
			format++;
			count += handle_format(*format, args);
		}
		else
			count += write(1, format, 1);
		format++;
	}
	va_end(args);
	return (count);
}
