#include "get_next_line.h"

char	*read_to_buffer(int fd, char *buffer)
{
	char	*tmp;
	int	bytes_read;

	tmp = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!tmp)
		return (NULL);
	bytes_read = 1;
	while (*buffer != '\n' && bytes_read > 0)
	{
		bytes_read = read(fd, tmp, );
		if (bytes_read == -1)
		{
			free(tmp);
			return (NULL);
		}
		tmp
		ft_strjoin(buffer, tmp);
	}
	free(tmp);
	return (buffer);
}

char	*extract_line(char *buffer)
{
	char	*line;
	int	count;

	if (!buffer)
		return (NULL);
	count = 0;
	while (*buffer != '\n' || *buffer != '\0')
		count++;
	line = malloc((count + 2) * sizeof(char));
	if (!line)
		return (NULL);
	while (*buffer != '\n')
	{
		*line = *buffer;
		line++;
		buffer++;
	}
	if (*buffer != '\n')
		*line = '\n';
	*line = '\0';
	return (line);
}

char	*update_line(char *buffer)
{
	char	*new_buffer;
	int	i;

	i = 0;
	while (buffer[i] != '\n')
		i++;
	if (!buffer[i])
	{
		free(buffer);
		return (NULL);
	}
	new_buffer = malloc((ft_strlen(buffer) - i) * sizeof(char));
	if (!new_buffer)
		return (NULL);
	while (buffer[i])
	{
		*new_buffer = buffer[i];
		i++;
	}
	free(buffer);
	return (new_buffer);
}

char	*get_next_line(int fd)
{
	static char	*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = read_to_buffer(fd, buffer);
	line = extract_line(buffer);
	buffer = update_buffer(buffer);
	return (line);
}
