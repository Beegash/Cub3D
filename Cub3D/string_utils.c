#include "cub3d.h"

char *ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	return (NULL);
}

// Güvenli kopyalama
size_t ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t i;

	i = 0;
	if (size > 0)
	{
		while (src[i] && i < size - 1)
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	while (src[i])
		i++;
	return (i);
}

char *ft_trimend(const char *s1, const char *set)
{
    size_t len;
    char *trimmed;
    size_t i;

    if (!s1)
        return (NULL);
    len = ft_strlen(s1);
    // Sadece sondaki karakterleri set içindeyse kırp
    while (len > 0 && strchr(set, s1[len - 1]))
        len--;
    trimmed = malloc(len + 1);
    if (!trimmed)
        return (NULL);
    i = 0;
    while (i < len)
    {
        trimmed[i] = s1[i];
        i++;
    }
    trimmed[i] = '\0';
    return (trimmed);
}
