#include "get_next_line.h"

char	*ft_read(int fd, char *str)
{
	char	*buff;
	int		rd_byte;

	buff = malloc ((BUFFER_SIZE + 1) * sizeof(char));
	if (!buff)
		return (0);
	rd_byte = 1;
	while (ft_find(str) && rd_byte != 0)
	{
		rd_byte = read(fd, buff, BUFFER_SIZE);
		if (rd_byte == -1)
		{
			free(buff);
			return (0);
		}
		buff[rd_byte] = '\0';
		str = ft_strjoin(str, buff);
	}
	free(buff);
	return (str);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*lft_str;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	lft_str = ft_read(fd, lft_str);
	if (!lft_str)
		return (0);
	line = ft_line(lft_str);
	lft_str = ft_clean(lft_str);
	return (line);
}
/*
#include <fcntl.h>
#include <stdio.h>
int main()
{
	int fd = open("a.txt", O_CREAT | O_RDWR, 0777);
	char *s = get_next_line(fd);
	printf("%s", s);
	free(s);
	s = get_next_line(fd);
	free(s);
	printf("%s", s);
	s = get_next_line(fd);
	printf("%s", s);
	free(s);
}*/