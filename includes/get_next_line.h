#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>
# include <stddef.h>
# include "libft.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char	*read_to_buffer(int fd, char *buffer);
char	*extract_line(char *buffer);
char	*update_buffer(char *buffer);
char	*get_next_line(int fd);

#endif
