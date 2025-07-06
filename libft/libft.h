#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <limits.h>
# include <stdarg.h>
# include <fcntl.h>

/* Part 1: Libc functions */
int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
size_t	ft_strlen(char *str);
void	*ft_memset(void *b, int c, size_t len);
void	ft_bzero(void *s, size_t n);
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_memmove(void *dst, const void *src, size_t len);
size_t	ft_strlcpy(char *dst, const char *src, unsigned int size);
size_t	ft_strlcat(char *dst, const char *src, unsigned int size);
int		ft_strncmp(char *s1, const char *s2, unsigned int n);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
char	*ft_strnstr(const char *haystack, const char *needle, unsigned int len);
int		ft_atoi(const char *str);
void	*ft_calloc(unsigned int count, unsigned int size);
char	*ft_strdup(const char *s);

/* Part 2: Additional functions */
char	*ft_substr(char const *s, unsigned int start, unsigned int len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char *));
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
char	*ft_itoa(int n);
char	**ft_split(char const *s, char c);

/* GNL */
# define BUFFER_SIZE 42
char	*get_next_line(int fd);

/* printf */
int		ft_printf(const char *format, ...);
int		print_char(char c);
int		print_string(char *s);
int		print_int(int n);
int		print_unsigned(unsigned int n);
int		print_hex(unsigned int n, int uppercase);
int		print_ptr(unsigned long long ptr);
int		handle_format(char specifier, va_list args);

#endif
