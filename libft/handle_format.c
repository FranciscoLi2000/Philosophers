#include "libft.h"

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
	else if (specifier == 'p')
		count += print_ptr(va_arg(args, unsigned long long));
	else if (specifier == '%')
		count += print_char('%');
	return (count);
}
