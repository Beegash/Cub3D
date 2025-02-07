#include "cub3d.h"

void *ft_memset(void *b, int c, size_t len)
{
    unsigned char *ptr;
    
    ptr = (unsigned char *)b;
    while (len--)
        *ptr++ = (unsigned char)c;
    return (b);
} 