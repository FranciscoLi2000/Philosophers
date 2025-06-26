#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "libft.h"
# include <stdarg.h>

int	print_char(int c);
int	print_string(char *s);
int	print_int(int n);
int	print_unsigned(unsigned int n);
int	print_hex(unsigned int n, int uppercase);
int	print_ptr(unsigned long long ptr);
int	handle_format(char specifier, va_list args);
int	ft_printf(const char *format, ...);

#endif
