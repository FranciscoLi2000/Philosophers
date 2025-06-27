#include "get_next_line.h"

char	*read_to_buffer(int fd, char *buffer)
{
	char	*tmp;
	int		bytes_read;

	tmp = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!tmp)
		return (NULL);
	bytes_read = 1;
	while (!ft_strchr(buffer, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, tmp, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(tmp);
			return (NULL);
		}
		tmp[bytes_read] = '\0';
		buffer = ft_strjoin(buffer, tmp);
	}
	free(tmp);
	return (buffer);
}

char	*extract_line(char *buffer)
{
	char	*line;
	int		i;

	if (!buffer || buffer[0] == '\0')
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		i += 2;
	else
		i += 1;
	line = malloc(i * sizeof(char));
	if (!line)
		return (NULL);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
	{
		line[i] = buffer[i];
		i++;
	}
	if (buffer[i] == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return (line);
}

char	*update_buffer(char *buffer)
{
	char	*new_buffer;
	int		i;
	int		j;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
	{
		free(buffer);
		return (NULL);
	}
	i++;
	new_buffer = malloc((ft_strlen(buffer) - i + 1) * sizeof(char));
	if (!new_buffer)
		return (NULL);
	while (buffer[i])
		new_buffer[j++] = buffer[i++];
	new_buffer[j] = '\0';
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
